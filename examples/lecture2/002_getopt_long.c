#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int c, config_id, opt_idx, should_create = 0;

    /*
    struct option {
        const char *name; // имя без "-"
        int has_arg; // 0 - без аргумента, 1 - обязательный, 2 - необязательный
        int *flag; // NULL -> getopt возвращает val
                   // иначе -> getopt возвращает 0, *flag = val
        int val;
    }
    */

    struct option options[] = {
        { "help",      no_argument,       NULL,           'h' },
        { "config_id", required_argument, NULL,           'c' },
        { "create",    optional_argument, &should_create,  1 },
        { NULL,        0,                 NULL,            0 }
    };

    while ( (c = getopt_long(argc, argv, "-:c:h", options, &opt_idx)) != -1 ) {
        printf("c=%c, optopt=%c, optind=%d, optarg=%s, opt_idx=%d\n", c, optopt, optind, optarg, opt_idx);
        switch (c) {
        case 0: 
            printf("long option %s", options[opt_idx].name);
            if (optarg) { printf(" with arg %s", optarg); }
            printf("\n");
            break;
        
        case 1:
            printf("non-option argument %s\n", optarg);
            break;

        case 'c':
            printf("Option %c with arg %s\n", c, optarg);
            config_id = atoi(optarg);
            break;
        
        case 'h':
            printf("Usage: %s [--help|--config <id>|--create [opt_id]] [-c <id>|-h]\n", argv[0]);
            break;
        
        case '?':
            printf("Unknown option %c\n", optopt);
            break;

        case ':':
            printf("Missing argument for %c\n", optopt);
            break;

        default:
            break;
        }
    }

    printf("config_id=%d, should_create=%d\n", config_id, should_create);
    printf("Remained args\n");
    while (optind < argc) {
        printf("%s\n", argv[optind]);
        ++optind;
    }
}