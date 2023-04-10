#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

enum command_mode {
  mode_background,
  mode_foreground
};

struct command{
  int argc;
  char**argv;
  char* cmdline;
  enum command_mode mode;
};

/* create a command structure from a command line  */
struct command* extract_command(char* cmdline) {
  struct command* c = malloc(sizeof(struct command));
  c->argc = 0;
  c->argv = NULL;
  c->cmdline = malloc(sizeof(char)*(strlen(cmdline)+1));
  strcpy(c->cmdline, cmdline);

  /* first, let's count the number of parameters */
  char* token = strtok(cmdline, " ");
  while(token) {
    c->argc++;
    token = strtok(NULL, " ");
  }
  /* strtok modified cmdline, so let's restore it */
  strcpy(cmdline, c->cmdline);

  /* now, extract the parameters */
  c->argv = malloc(sizeof(char*) * (c->argc+1));
  c->argv[0] = strtok(cmdline, " ");
  int i;
  for(i=1; i<c->argc; i++) {
    c->argv[i] = strtok(NULL, " ");
  }

  if(c->argc && strcmp("&", c->argv[c->argc-1]) == 0) {
    c->argc--;
    c->mode = mode_background;
  } else {
    c->mode = mode_foreground;
  }
  c->argv[c->argc] = NULL;

  return c;
}

void execute_command(struct command* c) {
    if(c->argc == 0) {
        /* empty command, nothing to do */
        return;
    }

    pid_t pid = fork();

    if(pid == -1) {
        /* error: fork failed */
        perror("fork");
        return;
    }

    if(pid == 0) {
        /* this is the child process */
        execvp(c->argv[0], c->argv);
        /* execvp only returns on error */
        perror(c->argv[0]);
        exit(EXIT_FAILURE);
    }

    /* this is the parent process */
    if(c->mode == mode_foreground) {
        /* wait for the child process to complete */
        int status;
        waitpid(pid, &status, 0);

        /* check if the child process terminated normally */
        if(WIFEXITED(status)) {
            printf("Process %d exited with status %d\n", pid, WEXITSTATUS(status));
            printf("Command: %s\n", c->cmdline);
        }
    } else {
        /* background process */
        printf("Background process launched with PID %d\n", pid);
    }
}


int main(int argc, char** argv){

  do {
    char *cmdline = NULL;
    /* print a prompt, and return a buffer that contains the user command */
    cmdline = readline("mysh $ ");

    if(! cmdline) {
      /* received EOF */
      return EXIT_SUCCESS;
    }

    /* extract the command, and execute it */
    struct command* cmd = extract_command(cmdline);
    execute_command(cmd);

    if(cmd->argc > 0 &&
       strcmp(cmd->argv[0] , "exit") == 0) {
      return EXIT_SUCCESS;
    }


    /* cleanup */
    free(cmd);
    free(cmdline);
    cmdline = NULL;

      /* check for terminated background processes */
      pid_t pid = waitpid(-1, &status, WNOHANG);
      while(pid > 0) {
          printf("Background process %d terminated\n", pid);
          pid = waitpid(-1, &status, WNOHANG);
      }


  } while(1);

  return EXIT_SUCCESS;
}
