#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include "server_code_etudiant.h"

struct client_node
{
    struct client_node *next;
    client_t *client;
};

struct client_node *clients;

/* Strip CRLF */
void strip_newline(char *s)
{
    while (*s != '\0')
    {
        if (*s == '\r' || *s == '\n')
        {
            *s = '\0';
        }
        s++;
    }
}

client_t *get_client_from_fd(int fd)
{
    struct client_node *cur = clients;
    while (cur != NULL)
    {
        if (cur->client->fd == fd)
        {
            return cur->client;
        }
        cur = cur->next;
    }
    return NULL;
}

client_t *get_client_from_name(char *name)
{
    struct client_node *cur = clients;
    while (cur != NULL)
    {
        if (strcmp(cur->client->name, name) == 0)
        {
            return cur->client;
        }
        cur = cur->next;
    }
    return NULL;
}

void server_init()
{
    clients = NULL;
}

/* Add client to chained list */
void queue_add(client_t *cl)
{
    struct client_node *new_node = malloc(sizeof(struct client_node));
    new_node->next = clients;
    new_node->client = cl;
    clients = new_node;
}

/* Delete client from queue */
void queue_delete(client_t *client)
{
    if (clients == NULL)
    {
        return;
    }
    struct client_node *previous_node = NULL;
    struct client_node *cur_node = clients;
    struct client_node *next_node = clients->next;
    while (next_node != NULL && cur_node->client->uid != client->uid)
    {
        previous_node = cur_node;
        cur_node = next_node;
        next_node = next_node->next;
    }
    if (next_node == NULL && cur_node->client->uid != client->uid)
    {
        return;
    }
    if (previous_node == NULL)
    {
        struct client_node *new_start = clients->next;
        free(clients);
        clients = new_start;
    }
    // Normal behavior, we delete the cur_node and make the junction
    free(cur_node);
    previous_node->next = next_node;
}

/* Send a message to a client */
void send_message(char *s, client_t *client)
{
    fwrite(s, sizeof(char), strlen(s), client->client_conn);
}

/* Send message to all clients */
void send_message_all(char *s)
{
    struct client_node *cur = clients;
    while (cur != NULL)
    {
        send_message(s, cur->client);
        cur = cur->next;
    }
}

void assign_default_name(client_t *cli)
{
    // Init random and variables
    srand(time(0));
    char name[NAME_MAX_LENGTH];

    FILE *names;
    names = fopen("default_names.txt", "r");
    // On doit calculer le nombre de ligne du fichier, sachant que toutes les lignes sont formaté à NAME_MAX_LENGTH caratères
    fseek(names, 0, SEEK_END);    // déplace le curseur à la position de la fin du fichier (sans offset)
    int file_size = ftell(names); // Donne le nombre d'octets depuis le début du fichier à la position actuelle.
    int nb_lines = file_size / NAME_MAX_LENGTH;

    do
    {
        // On génère la ligne ou on va chercher le nom
        int line_selected = rand() % nb_lines;
        fseek(names, NAME_MAX_LENGTH * line_selected, SEEK_SET);
        // fgets va lire un mot de 32 octets, alors que fread va lire un mot de size*nmmeb octets !
        // fread(word, sizeof(char), WORD_LEN, f);
        fgets(name, 32, names);
    } while (get_client_from_name(name) != NULL);

    sprintf(cli->name, "%s", name);
    fclose(names);
}

/* this function is called when a client connects to the server */
void say_hello(client_t *cli)
{
    char buff_out[1024];
    /* choose a default name */
    assign_default_name(cli);
    sprintf(buff_out, "* %s joins the chatroom\n", cli->name);
    send_message_all(buff_out);
}

void process_cmd_msg(client_t *client,
                     char *param)
{
    char *dest = strsep(&param, " ");
    if (!dest)
    {
        send_message("* to who ?\n", client);
        return;
    }

    char buffer[1024];
    sprintf(buffer, "[PM][%s --> %s] %s\n", client->name, dest, param);
    client_t *to = get_client_from_name(dest);
    if (!to)
    {
        send_message("* %s does not exist!\n", client);
    }
    else
    {
        send_message(buffer, to);
        send_message(buffer, client);
    }
}

void process_cmd_help(client_t *client)
{
    char buff_out[1024];
    sprintf(buff_out, "/help     Show help\n");
    strcat(buff_out, "/msg      <name> <message> Send private message\n");
    strcat(buff_out, "/ping     Server test\n");
    strcat(buff_out, "/quit     Quit chatroom\n");
    send_message(buff_out, client);
}

void process_cmd_ping(client_t *client,
                      char *param)
{
    send_message("* pong\n", client);
}

void process_cmd_me(client_t *client, char *param)
{
    char buff_out[1024];
    sprintf(buff_out, "* %s %s", client->name, param);
    send_message(buff_out, client);
}

void process_cmd_names(client_t *client, char *param)
{
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        char buff_out[1024];
        sprintf(buff_out, "* There are %d clients \n", cli_count);
        send_message(buff_out, client);
        struct client_node *cur = clients;
        while (cur != NULL)
        {
            sprintf(buff_out, "  * client %d | %s \n", cur->client->uid, cur->client->name);
            send_message(buff_out, client);
            cur = cur->next;
        }

        exit(EXIT_SUCCESS);
    }
}

void server_finalize(int signo)
{
}

void handle_incoming_cmd(client_t *cli)
{
    char buff_out[1024];
    char buff_in[1024];

    if (fgets(buff_in, 1024 * sizeof(char), cli->client_conn) == 0)
    {
        if (!feof(cli->client_conn))
        {
            perror("read failed");
            abort();
        }
        else
        {
            printf("Client %s disconnected\n", cli->name);
            queue_delete(cli);
            return;
        }
    }

    strip_newline(buff_in);

    /* Ignore empty buffer */
    if (!strlen(buff_in))
    {
        printf("Empty message\n");
    }

    /* Special options */
    char *cmd_line = buff_in;
    if (buff_in[0] == '/')
    {
        char *command;
        // coupe au niveau du délimiteur, cmd_line récupère bien juste le texte après la commande.
        command = strsep(&cmd_line, " ");
        if (!strcmp(command, "/quit"))
        {
            return;
        }
        else if (!strcmp(command, "/ping"))
        {
            process_cmd_ping(cli, cmd_line);
        }
        else if (!strcmp(command, "/msg"))
        {
            process_cmd_msg(cli, cmd_line);
        }
        else if (!strcmp(command, "/me"))
        {
            process_cmd_me(cli, cmd_line);
        }
        else if (!strcmp(command, "/names"))
        {
            process_cmd_names(cli, cmd_line);
        }
        else
        {
            /* /help or unknown command */
            process_cmd_help(cli);
        }
    }
    else
    {
        /* Send message */
        sprintf(buff_out, "[%s] %s\n", cli->name, cmd_line);
        send_message_all(buff_out);
    }
}
