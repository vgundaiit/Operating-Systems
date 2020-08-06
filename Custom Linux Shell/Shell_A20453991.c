#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
/* Varun's Comment : errno.h is added to get the error number when a command's execution fails
*/
#include <errno.h>
/* Varun's Comment : sys/wait.h is added to remove the following warning:
    
    warning: implicit declaration of function ‘wait’

*/
#include <sys/wait.h> 

// Simplifed xv6 shell.

#define MAXARGS 10

// All commands have at least a type. Have looked at the type, the code
// typically casts the *cmd to some specific cmd type.
struct cmd {
  int type;          //  ' ' (exec), | (pipe), '<' or '>' for redirection
};

struct execcmd {
  int type;              // ' '
  char *argv[MAXARGS];   // arguments to the command to be exec-ed
};

struct redircmd {
  int type;          // < or > 
  struct cmd *cmd;   // the command to be run (e.g., an execcmd)
  char *file;        // the input/output file
  int mode;          // the mode to open the file with
  int fd;            // the file descriptor number to use for the file
};

struct pipecmd {
  int type;          // |
  struct cmd *left;  // left side of pipe
  struct cmd *right; // right side of pipe
};

/*
    Varun's Comment: sequenceCmd & parallelCmd structures defined below are used later in the
    code
 */ 
struct sequenceCmd{
  int type; // ;
  struct cmd *left;  // left side of sequence operator ;
  struct cmd *right; // right side of sequence operator ;
};

struct parallelCmd{
  int type; // &
  struct cmd *left;  // left side of parallel operator &
  struct cmd *right; // right side of parallel operator &
};

int fork1(void);  // Fork but exits on failure.
struct cmd *parsecmd(char*);
static int ampStarted = 0;


// Execute cmd.  Never returns.
void
runcmd(struct cmd *cmd)
{
  int p[2], r,w;
  struct execcmd *ecmd;
  struct execcmd *elcmd; //left command to &
  struct pipecmd *pcmd;
  struct redircmd *rcmd;
  struct sequenceCmd *scmd;
  struct parallelCmd *plcmd;
  /*
  Varun's Comment: Variables wpid & status are used for the parent process to wait 
  for all the child processes to exit.
   */
  pid_t wpid;
  int status = 0;

  if(cmd == 0)
    exit(0);
  
  switch(cmd->type){
  default:
    fprintf(stderr, "unknown runcmd\n");
    exit(-1);
/*   
    Varun's Comment: Parallel commands case
*/
  case '&' : 
    plcmd = (struct parallelCmd*)cmd;
    elcmd = (struct execcmd*)plcmd->left;
    /*
        Varun's Comment: Executing the command on the left side of the ampersand by 
        forking a child process
    */
    if(fork1() == 0){
        execvp(elcmd->argv[0],elcmd->argv);  
    }
    /*
        Varun's Comment: Recursively calling runcmd to execute the next command 
    */
    runcmd(plcmd->right);
  break;

/*
    Varun's Comment: Sequence commands case
*/

  case ';':
    scmd = (struct sequenceCmd*)cmd;   
    /* 
        Varun's Comment: Execute command on the left side of the semicolon by forking a child 
        process
    */ 
    ecmd = (struct execcmd*)scmd->left;
    if(ecmd->argv[0] == 0)
      exit(0);
    if(fork1() == 0){
        execvp(ecmd->argv[0],ecmd->argv);
    }
    /*
        Varun's Comment: Wait for all the child processes created so far to terminate
    */
    while ((wpid = wait(&status)) > 0);
    
    /*
        Varun's Comment: Recursively calling runcmd to execute the next command 
    */
    runcmd(scmd->right);
    
    break;

/*
    Varun's Comment: ' ' => Case to cover last command in the command list or if only one command 
    is given
*/

  case ' ':
    
    ecmd = (struct execcmd*)cmd;
    if(ecmd->argv[0] == 0)
      exit(0);
    if(fork1() == 0){
        /*
            Varun's comment: Print out if the execvp fails due to some error and print the 
            error number
        */
      if((execvp(ecmd->argv[0],ecmd->argv)) < 0) {
        fprintf(stderr,"There is an error occured for the command %s and the error number is %d\n",ecmd->argv[0],errno);  
      };
    }
    
    /*
        Varun's comment: Wait for all the child processes to terminate
    */
    
    while ((wpid = wait(&status)) > 0);
  break;

  }    
  exit(0);
}

int
getcmd(char *buf, int nbuf)
{
  
  if (isatty(fileno(stdin)))
    fprintf(stdout, "$ ");
  memset(buf, 0, nbuf);
  fgets(buf, nbuf, stdin);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}

/*
    Varun's comment: lastCharInCmd get the last character of the command chain given in the command line
    interface
 */
char lastCharInCmd(char *c){
  while(*c != '\0'){
    c++;
  }
  /*
    Varun's comment: remove white spaces and find the last significant character in the cmd
   */
  c--;
  while(*c == '\t' | *c == '\n' | *c == ' ' | *c == '\r' | *c == '\v'){
    c--;
  }
  return *c;
}

/*
    Varun's comment: "irregularCommand" Function : Following cases are considered invalid:
    1. cmd1&; // No command between & and ;
    2. cmd1;& // No command between ; and &
    3. cmd1;; // No command between ; and ;
    4. cmd1&& // No command between & and &

    If the user prints any of such commands, this function will return 1.
 */
int irregularCommand(char *buf){
  int i = 0;
  while(buf[i+1] != '\0'){
    if((buf[i] == ';' && buf[i+1] == '&') || (buf[i] == '&' && buf[i+1] == ';') || 
        (buf[i] == ';' && buf[i+1] == ';') || (buf[i] == '&' && buf[i+1] == '&') )
    {
      return 1;
    }
    i++; 
  }
  return 0;
}

int
main(void)
{
  static char buf[100];
  int fd, r;
  pid_t wpid;
  int status = 0;

  // Read and run input commands.
  while(getcmd(buf, sizeof(buf)) >= 0){
    if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' '){
      // Clumsy but will have to do for now.
      // Chdir has no effect on the parent if run in the child.
      buf[strlen(buf)-1] = 0;  // chop \n
      if(chdir(buf+3) < 0)
        fprintf(stderr, "cannot cd %s\n", buf+3);
      continue;
    }
    /*
        Varun's comment: This takes care of the case where last character in the command string is &
     */
    if(lastCharInCmd(buf) == '&'){
      fprintf(stderr, "Command must not terminate by &\n");
      continue;
    }
    /*
        Varun's comment: This takes care of the case where command string contains ; and & next to each other 
        without any command in between them
    */
    if(irregularCommand(buf)){
      fprintf(stderr,"Command is irregular. Please enter command correctly\n");
      continue;
    };
    /*
        Varun's comment: Fork and create a process and run the command string 
        after parsing it.
    */
    if(fork1() == 0)
      runcmd(parsecmd(buf));
    /*
        Varun's comment:  Wait until all the child processes exit.
     */
    while ((wpid = wait(&status)) > 0);
  }
  exit(0);
}

int
fork1(void)
{
  int pid;
  
  pid = fork();
  if(pid == -1)
    perror("fork");
  return pid;
}

struct cmd*
execcmd(void)
{
  struct execcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = ' ';
  return (struct cmd*)cmd;
}

struct cmd*
redircmd(struct cmd *subcmd, char *file, int type)
{
  struct redircmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = type;
  cmd->cmd = subcmd;
  cmd->file = file;
  cmd->mode = (type == '<') ?  O_RDONLY : O_WRONLY|O_CREAT|O_TRUNC;
  cmd->fd = (type == '<') ? 0 : 1;
  return (struct cmd*)cmd;
}

struct cmd*
pipecmd(struct cmd *left, struct cmd *right)
{
  struct pipecmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = '|';
  cmd->left = left;
  cmd->right = right;
  return (struct cmd*)cmd;
}

/*
    Varun's comment: Create a structure sequenceCmd type variable and return it
 */
struct cmd*
sequenceCmd(struct cmd *left, struct cmd *right)
{
  struct sequenceCmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = ';';
  cmd->left = left;
  cmd->right = right;
  return (struct cmd*)cmd;
}

/*
    Varun's comment: Create a structure parallelCmd type variable and return it
*/
struct cmd*
parallelCmd(struct cmd *left, struct cmd *right)
{
  struct parallelCmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = '&';
  cmd->left = left;
  cmd->right = right;
  return (struct cmd*)cmd;
}

// Parsing

char whitespace[] = " \t\r\n\v";

/*
    Varun's comment: Symbols that have to be identified in the command string
 */
char symbols[] = ";&";


int
gettoken(char **ps, char *es, char **q, char **eq)
{
  char *s;
  int ret;
  
  s = *ps; 
  
  while(s < es && strchr(whitespace, *s))
    s++;

  if(q)
    *q = s; 
  
  ret = *s; 
  
  switch(*s){
    
    case 0:
      break;
    /*
        Varun's comment: Adding the cases to cover for ';' and '&'
     */
    case ';':
      s++;
      break;
    case '&':
      s++;
      break;
    
    default:
      ret = 'a'; 
      while(s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
        s++;
      break;
  }
  if(eq)
    *eq = s; 

  while(s < es && strchr(whitespace, *s))
    s++;
  *ps = s;
  return ret;
}

int
peek(char **ps, char *es, char *toks)
{
  char *s;
  
  s = *ps;
  while(s < es && strchr(whitespace, *s))
    s++;
  *ps = s;
  return *s && strchr(toks, *s); 
}

struct cmd *parseline(char**, char*);
struct cmd *parsepipe(char**, char*);
/*
    Varun's comment :  Added function parseSequence that parses the command string
 */
struct cmd *parseSequence(char**, char*); 
struct cmd *parseexec(char**, char*);

// make a copy of the characters in the input buffer, starting from s through es.
// null-terminate the copy to make it a string.
char 
*mkcopy(char *s, char *es)
{
  int n = es - s;
  char *c = malloc(n+1);
  assert(c);
  strncpy(c, s, n);
  c[n] = 0;
  return c;
}

struct cmd*
parsecmd(char *s)
{
  char *es;
  struct cmd *cmd;

  es = s + strlen(s);
  cmd = parseline(&s, es); 
  peek(&s, es, "");
  if(s != es){
    fprintf(stderr, "leftovers: %s\n", s);
    exit(-1);
  }
  return cmd;
}

struct cmd*
parseline(char **ps, char *es)
{
  struct cmd *cmd;
  /*
    Varun's comment: call parseSEquence function here
   */
  cmd = parseSequence(ps,es);
  return cmd;
}

/*
    Varun's comment: This function parseSequence the command string and checks for ; and & and build the "cmd". 
    Also, this recursively calls itself to find all of them and builds "cmd"
 */
struct cmd*
parseSequence(char **ps, char *es)
{
  struct cmd *cmd;

  cmd = parseexec(ps, es);
  if(peek(ps, es, ";")){
    gettoken(ps, es, 0, 0);
    cmd = sequenceCmd(cmd, parseSequence(ps, es));
  }
  else if (peek(ps,es,"&")){
    gettoken(ps, es, 0, 0);
    cmd = parallelCmd(cmd, parseSequence(ps, es));  
  }
  return cmd;
}



struct cmd*
parsepipe(char **ps, char *es)
{
  struct cmd *cmd;

  cmd = parseexec(ps, es);
  if(peek(ps, es, "|")){
    gettoken(ps, es, 0, 0);
    cmd = pipecmd(cmd, parsepipe(ps, es));
  }
  return cmd;
}

struct cmd*
parseredirs(struct cmd *cmd, char **ps, char *es)
{
  int tok;
  char *q, *eq;

  while(peek(ps, es, "<>")){
    tok = gettoken(ps, es, 0, 0);
    if(gettoken(ps, es, &q, &eq) != 'a') {
      fprintf(stderr, "missing file for redirection\n");
      exit(-1);
    }
    switch(tok){
    case '<':
      cmd = redircmd(cmd, mkcopy(q, eq), '<');
      break;
    case '>':
      cmd = redircmd(cmd, mkcopy(q, eq), '>');
      break;
    }
  }
  return cmd;
}

struct cmd*
parseexec(char **ps, char *es) 
{
  char *q, *eq;
  int tok, argc;
  struct execcmd *cmd;
  struct cmd *ret;
  
  ret = execcmd();
  cmd = (struct execcmd*)ret;

  argc = 0;
  /*
    Varun's comment: Look for ; and & in the command string and build the cmd accordingly
    */
  
  while(!peek(ps, es, ";&")){
  
    if((tok=gettoken(ps, es, &q, &eq)) == 0)
      break;
    if(tok != 'a') {
      fprintf(stderr, "syntax error\n");
      exit(-1);
    }
  
    cmd->argv[argc] = mkcopy(q, eq); 
    argc++;
    if(argc >= MAXARGS) {
      fprintf(stderr, "too many args\n");
      exit(-1);
    }
  }
  cmd->argv[argc] = 0;
  return ret;
}
