/* use this file to test sendmail and options */
#include <stdio.h>
#include <string.h>

static void readProperties() {
    printf("test: readProperties\n");
    /* TODO read property file in the same dir with name pptpd-sendmail.properties */

    /* TODO test if file exists */

    char textIn[100]; 

    FILE *fp;
    fp=fopen("foo.properties", "r");
    fscanf(fp, "%[^\n]\n", textIn);
    fclose(fp);

    printf("test: readProperties result: %s\n", textIn);
}

static void testSendMail()
{
  char cmd[100];  // to hold the command.
  char to[] = "example@example.com"; // email id of the recepient.
  char body[] = "SO rocks";    // email body.
  char tempFile[100];     // name of tempfile.

  //char template_name[]="/tmp/sendmailXXXXXX";
  //int temp_fd;
  //temp_fd=mkstemp(template_name);
  strcpy(tempFile,tempnam("/tmp","sendmail")); // generate temp file name.
  
  FILE *fp = fopen(tempFile,"w"); // open it for writing.
  //FILE *fp = fdopen(temp_fd,"w"); // open it for writing.
  fprintf(fp,"%s\n",body);        // write body to it.
  fclose(fp);             // close it.
  
  printf("sendmail %s < %s\n\n",to,tempFile); // log command for debugging
  printf("%s\n\n",body); // log body for debugging

  sprintf(cmd,"sendmail %s < %s",to,tempFile); // prepare command.
  system(cmd);     // execute it.
}

int main() {
  printf("test: main\n");

  readProperties();

  testSendMail();

  return(0);
}