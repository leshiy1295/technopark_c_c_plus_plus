#if defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>


#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/http.h>

static int ignore_cert = 0;
static int ipv6 = 0;
static int ipv4 = 0;

static void
http_request_done(struct evhttp_request *req, void *ctx)
{
    char buffer[256];
    int nread;

    if (!req || !evhttp_request_get_response_code(req)) {
        // If req is NULL, it means an error occurred
        fprintf(stderr, "some request failed - no idea which one though!\n");
        return;
    }

    fprintf(stderr, "Response line: %d %s\n",
            evhttp_request_get_response_code(req),
            evhttp_request_get_response_code_line(req));

    while ((nread = evbuffer_remove(evhttp_request_get_input_buffer(req),
                                    buffer, sizeof(buffer)))
           > 0) {
        /* These are just arbitrary chunks of 256 bytes.
         * They are not lines, so we can't treat them as such. */
        fwrite(buffer, nread, 1, stdout);
    }
}

static void syntax(void)
{
    fputs("Syntax:\n", stderr);
    fputs("   https-client -url <https-url> [-data data-file.bin] [-ignore-cert] [-4] [-6] [-retries num] [-timeout sec] [-crt crt]\n", stderr);
    fputs("Example:\n", stderr);
    fputs("   https-client -url https://ip.appspot.com/\n", stderr);
}

static void err(const char *msg)
{
    fputs(msg, stderr);
}

int
main(int argc, char **argv)
{
    const char *url = NULL, *data_file = NULL;
    char uri[256];

    int retries = 0;
    int timeout = -1;

    for (int i = 1; i < argc; i++) {
        if (!strcmp("-url", argv[i])) {
            if (i < argc - 1) {
            }
            url = argv[i + 1];
        }
    }

    if (!url) {
        syntax();
        return -1;
    }

    struct evhttp_uri *http_uri = evhttp_uri_parse(url);
    if (http_uri == NULL) {
        err("malformed url\n");
        return -1;
    }

    const char *scheme = evhttp_uri_get_scheme(http_uri);
    if (scheme == NULL || strcasecmp(scheme, "http") != 0) {
        err("url must be http\n");
        return -1;
    }

    const char *host = evhttp_uri_get_host(http_uri);
    if (host == NULL) {
        err("url must have a host\n");
        return -1;
    }

    int port = evhttp_uri_get_port(http_uri);
    if (port == -1) {
        port = (strcasecmp(scheme, "http") == 0) ? 80 : 443;
    }

    const char *path = evhttp_uri_get_path(http_uri);
    if (strlen(path) == 0) {
        path = "/";
    }

    const char *query = evhttp_uri_get_query(http_uri);
    if (query == NULL) {
        snprintf(uri, sizeof(uri) - 1, "%s", path);
    } else {
        snprintf(uri, sizeof(uri) - 1, "%s?%s", path, query);
    }
    uri[sizeof(uri) - 1] = '\0';


    // Create event base
    struct event_base *base = event_base_new();
    if (!base) {
        perror("event_base_new()");
        return -1;
    }

    struct bufferevent *bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);

    if (bev == NULL) {
        fprintf(stderr, "bufferevent_openssl_socket_new() failed\n");
        return -1;
    }

    // For simplicity, we let DNS resolution block. Everything else should be
    // asynchronous though.
    struct evhttp_connection *evcon = evhttp_connection_base_bufferevent_new(base, NULL, bev,
                                                                             host, port);
    if (evcon == NULL) {
        fprintf(stderr, "evhttp_connection_base_bufferevent_new() failed\n");
        return -1;
    }

    if (ipv4) {
        evhttp_connection_set_family(evcon, AF_INET);
    }
    if (ipv6) {
        evhttp_connection_set_family(evcon, AF_INET6);
    }

    if (retries > 0) {
        evhttp_connection_set_retries(evcon, retries);
    }
    if (timeout >= 0) {
        evhttp_connection_set_timeout(evcon, timeout);
    }

    // Fire off the request
    struct evhttp_request *req = evhttp_request_new(http_request_done, bev);
    if (req == NULL) {
        fprintf(stderr, "evhttp_request_new() failed\n");
        return -1;
    }

    struct evkeyvalq *output_headers = evhttp_request_get_output_headers(req);
    evhttp_add_header(output_headers, "Host", host);
    evhttp_add_header(output_headers, "Connection", "close");

    int r = evhttp_make_request(evcon, req, data_file ? EVHTTP_REQ_POST : EVHTTP_REQ_GET, uri);
    if (r != 0) {
        fprintf(stderr, "evhttp_make_request() failed\n");
        return -1;
    }

    event_base_dispatch(base);

    if (evcon)
        evhttp_connection_free(evcon);
    if (http_uri)
        evhttp_uri_free(http_uri);
    if (base)
        event_base_free(base);

    return 0;
}