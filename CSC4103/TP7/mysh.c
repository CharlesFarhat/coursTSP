#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

enum command_mode
{
  mode_background,
  mode_foreground
};

struct command
{
  int argc;
  char **argv;
  char *cmdline;
  enum command_mode mode;
};

struct bg_command
{
  struct bg_command *next;
  struct command *cmd;
  pid_t pid;
};

struct bg_command *bg_commands = NULL;

/* free a command structure */
void free_cmd(struct command *cmd)
{
  free(cmd->cmdline);
  free(cmd->argv);
  free(cmd);
}

/* process the completion of a background process */
void complete_process(pid_t pid)
{
  /* search for pid in the bg_commands list */
  struct bg_command *cmd = bg_commands;
  struct bg_command *prev = NULL;
  while (cmd)
  {
    if (cmd->pid == pid)
    {
      /* remove cmd from the list */
      if (prev)
      {
        prev->next = cmd->next;
      }
      else
      {
        /* cmd is the first of the list */
        bg_commands = cmd->next;
      }

      printf("[%d] Completed - %s\n", pid, cmd->cmd->cmdline);
      free_cmd(cmd->cmd);
      return;
    }

    /* jump to the next token */
    prev = cmd;
    cmd = cmd->next;
  }

  /* the process was not found in the list */
  printf("Error: process %d finished, but I can't find it\n", pid);
  abort();
}

/* create a command structure from a command line  */
struct command *extract_command(char *cmdline)
{
  struct command *c = malloc(sizeof(struct command));
  c->argc = 0;
  c->argv = NULL;
  c->cmdline = malloc(sizeof(char) * (strlen(cmdline) + 1));
  strcpy(c->cmdline, cmdline);

  /* first, let's count the number of parameters */
  char *token = strtok(cmdline, " ");
  while (token)
  {
    c->argc++;
    token = strtok(NULL, " ");
  }
  /* strtok modified cmdline, so let's restore it */
  strcpy(cmdline, c->cmdline);

  /* now, extract the parameters */
  c->argv = malloc(sizeof(char *) * (c->argc + 1));
  c->argv[0] = strtok(cmdline, " ");
  int i;
  for (i = 1; i < c->argc; i++)
  {
    c->argv[i] = strtok(NULL, " ");
  }

  if (c->argc && strcmp("&", c->argv[c->argc - 1]) == 0)
  {
    c->argc--;
    c->mode = mode_background;
  }
  else
  {
    c->mode = mode_foreground;
  }
  c->argv[c->argc] = NULL;

  return c;
}

/* execute a command */
void execute_command(struct command *c)
{
  if (c->argc > 0)
  {
    pid_t pid_child = fork();
    if (!pid_child)
    {
      execvp(c->argv[0], c->argv);
      /* execvp should not exit except in case of an error */
      fprintf(stderr, "Error running command %s: %s\n", c->argv[0], strerror(errno));
      exit(EXIT_FAILURE);
    }
    else
    {

      if (c->mode == mode_foreground)
      {
        int status;
        waitpid(pid_child, &status, 0);
      }
      else
      {
        /* background mode */
        struct bg_command *bg = malloc(sizeof(struct bg_command));
        bg->cmd = c;
        bg->pid = pid_child;
        bg->next = bg_commands;
        bg_commands = bg;
        printf("(in the background: process %d)\n", pid_child);
      }
    }
  }

  int status;
  pid_t pid = waitpid(0, &status, WNOHANG);
  if (pid > 0)
    complete_process(pid);
}

int main(int argc, char **argv)
{

  do
  {
    char *cmdline = NULL;
    /* print a prompt, and return a buffer that contains the user command */
    cmdline = readline("mysh $ ");

    /* extract the command, and execute it */
    struct command *cmd = extract_command(cmdline);
    execute_command(cmd);

    if (cmd->argc > 0 &&
        strcmp(cmd->argv[0], "exit") == 0)
    {

      free(cmd->cmdline);
      free(cmd->argv);
      free(cmd);
      free(cmdline);
      return EXIT_SUCCESS;
    }

    /* cleanup */

    if (cmd->mode == mode_foreground)
    {
      /* don't free the cmd structure if the command is launched in
   background mode */
      free_cmd(cmd);
    }

    free(cmdline);
    cmdline = NULL;
  } while (1);

  return EXIT_SUCCESS;
}