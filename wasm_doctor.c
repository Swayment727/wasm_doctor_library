#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>

void continue_execution(int pid) {
  ptrace(PTRACE_CONT, pid, NULL, NULL);

  int wait_status;
  int options = 0;
  waitpid(pid, &wait_status, options);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    return -1;
  }

  char *prog = argv[1];

  int pid = fork();
  if (pid == 0) {
    printf("Executing debuggee\n");
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    execl(prog, prog, NULL);
  } else if (pid >= 1) {
    int wait_status;
    int options = 0;
    waitpid(pid, &wait_status, options);

    char command[9];
    printf("Command: ");
    scanf("%s", command);
    printf("You entered %s\n", command);

    if (strncmp(command, "c", sizeof(command)) == 0 ||
        strncmp(command, "cont", sizeof(command)) == 0 ||
        strncmp(command, "continue", sizeof(command)) == 0) {
      continue_execution(pid);
    } else {
      printf("Not continuing\n");
    }
  }

  return 0;
}
