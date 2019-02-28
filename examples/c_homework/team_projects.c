#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MIN_TEAM_SIZE 3
#define MAX_TEAM_SIZE 4

enum group_id {APO_11 = 1, APO_12, APO_13};

struct student {
    char *name;
    char *surname;
    enum group_id group;
    char *link_to_git_profile;
};

struct team {
    char *name;
    struct {
        char *name;
        char *description;
    } project;
    size_t size;
    struct student *members;
    char *lead;
};

char input_char();

/* Returns NULL on failure */
char *input_string();

/* Returns 0 on failure */
int input_int();

/* Creates new team. Returns false on failure */
bool create(struct team **new_team_p);

bool is_space(char c);

/* Returns new allocated string or NULL on error */
char *get_trimmed_string(char const * const str);

/* Add backslashes before pattern characters in second parameter and print in quotes */
void print_escaped_string(const char * const str, const char symbols_to_escape[]);

/* Does not allocate new memory. Only returns pointer to substring in original string */
char *get_url_substring_without_schema(char * str);

/* Prints trimmed and espaced quoted string */
void print_formatted_string(const char * const string, const char symbols_to_escape[]);

/* Prints struct in JSON format in <code> tag */
void print_team_as_json(const struct team * const team);

void delete(struct team *team);

int main(int argc, char *argv[]) {
    struct team *new_team = NULL;
    bool success = create(&new_team);
    if (success) {
        print_team_as_json(new_team);
    }
    delete(new_team);
    return 0;
}

char input_char() {
    char c = '\0';
    int result = 0;
    do {
        result = scanf("%c", &c);
    } while (result != 1);
    return c;
}

char *input_string() {
    struct buffer {
        char *string;
        size_t size;
        size_t capacity;
    } buf = {NULL, 0, 0};
    char c = '\0';
    while (c = input_char(), c != EOF && c != '\n') {
        if (buf.size + 1 >= buf.capacity) {
            size_t new_capacity = !buf.capacity ? 1 : buf.capacity * 2;
            char *tmp = (char *)malloc((new_capacity + 1) * sizeof(char));
            if (!tmp) {
                if (buf.string) {
                    free(buf.string);
                }
                return NULL;
            }
            if (buf.string) {
                tmp = strcpy(tmp, buf.string);
                free(buf.string);
            }
            buf.string = tmp;
            buf.capacity = new_capacity;
        }
        buf.string[buf.size] = c;
        buf.string[buf.size + 1] = '\0';
        ++buf.size;
    }
    return buf.string;
}

int input_int() {
    char c = '\0';
    int result = 0;
    while (c = input_char(), c != EOF && c != '\n') {
        if (!(c >= '0' && c <= '9')) {
            char *buf = input_string(); /* Read to the end of the string */
            if (buf) {
                free(buf);
            }
            return 0;
        }
        result = result * 10 + c - '0';
    }
    return result;
}

bool create(struct team **new_team_p) {
    if (!new_team_p) {
        return false;
    }
    *new_team_p = (struct team *)malloc(sizeof(struct team));
    struct team *new_team = *new_team_p;
    if (!new_team) {
        return false;
    }
    new_team->name = NULL;
    new_team->name = NULL;
    new_team->project.name = NULL;
    new_team->project.description = NULL;
    new_team->size = 0;
    new_team->members = NULL;
    char *string = NULL;
    printf("Введите название команды\n");
    string = input_string();
    if (!string) {
        return false;
    }
    new_team->name = string;
    printf("Введите название проекта:\n");
    string = input_string();
    if (!string) {
        return false;
    }
    new_team->project.name = string;
    printf("Введите описание проекта:\n");
    string = input_string();
    if (!string) {
        return false;
    }
    new_team->project.description = string;
    int size = 0;
    do {
        printf("Введите размер команды (мин: %d - макс: %d):\n", MIN_TEAM_SIZE, MAX_TEAM_SIZE);
        size = input_int();
    } while (!(size >= MIN_TEAM_SIZE && size <= MAX_TEAM_SIZE));
    new_team->size = (size_t)size;
    new_team->members = (struct student *)malloc(new_team->size * sizeof(struct student));
    if (!new_team->members) {
        return false;
    }
    for (size_t i = 0; i < new_team->size; ++i) {
        new_team->members[i].name = NULL;
        new_team->members[i].surname = NULL;
        new_team->members[i].link_to_git_profile = NULL;
        new_team->members[i].group = APO_11;
        printf("Введите имя члена команды № %ld\n", i + 1);
        string = input_string();
        if (!string) {
            return false;
        }
        new_team->members[i].name = string;
        printf("Введите фамилию члена команды № %ld\n", i + 1);
        string = input_string();
        if (!string) {
            return false;
        }
        new_team->members[i].surname = string;
        int group = 0;
        do {
            printf("Введите номер группы члена команды № %ld (%d - %d):\n", i + 1, APO_11, APO_13);
            group = input_int();
        } while (!(group >= APO_11 && group <= APO_13));
        new_team->members[i].group = (size_t)group;
        printf("Введите ссылку на профиль в git-репозитории члена команды № %ld:\n", i + 1);
        string = input_string();
        if (!string) {
            return false;
        }
        new_team->members[i].link_to_git_profile = string;
    }
    int team_lead_idx = 0;
    do {
        printf("Введите номер члена команды, который будет выполнять роль лидера: (1..%d)\n", size);
        team_lead_idx = input_int();
    } while (!(team_lead_idx >= 1 && team_lead_idx <= size));
    struct student team_lead_member = new_team->members[team_lead_idx - 1];
    new_team->lead = (char *)malloc(strlen(team_lead_member.name) + strlen(team_lead_member.surname) + 2);
    if (!new_team->lead) {
      return false;
    }
    strcpy(new_team->lead, team_lead_member.name);
    strcat(new_team->lead, " ");
    strcat(new_team->lead, team_lead_member.surname);
    return true;
}

bool is_space(char c) {
    return c == ' ' || c == '\t';
}

char *get_trimmed_string(char const * const str) {
    if (!str) {
        return NULL;
    }
    char *tmp = (char *)malloc((strlen(str) + 1) * sizeof(char));
    if (!tmp) {
        return NULL;
    }
    tmp[0] = '\0';
    size_t i = 0;
    size_t j = 0;
    while (is_space(str[j])) {
        ++j;
    }
    while (str[j] != '\0') {
        while (str[j] != '\0' && !is_space(str[j])) {
            tmp[i++] = str[j++];
        }
        size_t first_space_index = j;
        while (is_space(str[j])) {
            ++j;
        }
        if (str[j] != '\0') {
            tmp[i] = '\0';
            tmp = strncat(tmp, str + first_space_index, j - first_space_index + 1);
            i += j - first_space_index + 1;
            ++j;
        }
    }
    tmp[i] = '\0';
    char *result = malloc((strlen(tmp) + 1) * sizeof(char));
    if (!result) {
        free(tmp);
        return NULL;
    }
    result = strcpy(result, tmp);
    free(tmp);
    return result;
}

void print_escaped_string(const char * const str, const char symbols_to_escape[]) {
    size_t i = 0;
    printf("\"");
    while (str[i] != '\0') {
        if (strchr(symbols_to_escape, str[i])) {
            printf("\\");
        }
        printf("%c", str[i]);
        ++i;
    }
    printf("\"");
}

char *get_url_substring_without_schema(char * str) {
    const char *pattern = "://";
    char *result = str;
    result = strstr(str, pattern);
    if (!result) {
        return str;
    }
    result += strlen(pattern);
    return result;
}

void print_formatted_string(const char * const string, const char symbols_to_escape[]) {
    char *trimmed_string = NULL;
    trimmed_string = get_trimmed_string(string);
    print_escaped_string(trimmed_string ? trimmed_string : string, symbols_to_escape);
    if (trimmed_string) {
        free(trimmed_string);
        trimmed_string = NULL;
    }
}

void print_team_as_json(const struct team * const team) {
    const char symbols_to_escape[] = "\\\"";
    printf("<code class=\"cpp\">");
    printf("{\t\n");
    printf("\t\"name\": ");
    print_formatted_string(team->name, symbols_to_escape);
    printf(",\n");
    printf("\t\"project\": {\n");
    printf("\t\t\"name\": ");
    print_formatted_string(team->project.name, symbols_to_escape);
    printf(",\n");
    printf("\t\t\"description\": ");
    print_formatted_string(team->project.description, symbols_to_escape);
    printf("\n");
    printf("\t},\n");
    printf("\t\"size\": %ld,\n", team->size);
    printf("\t\"members\": [\n");
    for (size_t i = 0; i < team->size; ++i) {
        printf("\t\t{\n");
        printf("\t\t\t\"name\": ");
        print_formatted_string(team->members[i].name, symbols_to_escape);
        printf(",\n");
        printf("\t\t\t\"surname\": ");
        print_formatted_string(team->members[i].surname, symbols_to_escape);
        printf(",\n");
        printf("\t\t\t\"group\": \"АПО-1%d\",\n", team->members[i].group);
        printf("\t\t\t\"link_to_git_profile\": ");
        /* A little hack - technopark portal transforms links with schema in html tags */
        char *url_without_schema = get_url_substring_without_schema(team->members[i].link_to_git_profile);
        print_formatted_string(url_without_schema, symbols_to_escape);
        printf("\n");
        printf("\t\t}%c\n", i + 1 != team->size ? ',' : ' ');
    }
    printf("\t],\n");
    printf("\t\"team_lead\": ");
    print_formatted_string(team->lead, symbols_to_escape);
    printf("\n}");
    printf("</code>\n");
}

void delete(struct team *team) {
    if (!team) {
        return;
    }
    if (team->name) {
        free(team->name);
    }
    if (team->project.name) {
        free(team->project.name);
    }
    if (team->project.description) {
        free(team->project.description);
    }
    for (size_t i = 0; i < team->size; ++i) {
        if (team->members[i].name) {
            free(team->members[i].name);
        }
        if (team->members[i].surname) {
            free(team->members[i].surname);
        }
        if (team->members[i].link_to_git_profile) {
            free(team->members[i].link_to_git_profile);
        }
    }
    if (team->members) {
        free(team->members);
    }
    if (team->lead) {
        free(team->lead);
    }
    free(team);
}

