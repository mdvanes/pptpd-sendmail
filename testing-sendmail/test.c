/* use this file to test sendmail and options */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static char readProperties(char to[]) {
    printf("test: readProperties\n");
    /* TODO read property file in the same dir with name pptpd-sendmail.properties */

    char textIn[1000];

    char fname[] = "pptpd-sendmail.properties";

    if( access( fname, F_OK ) != -1 ) {
        // file exists
        FILE *fp;
        fp=fopen(fname, "r");
        fscanf(fp, "%[^\n]\n", textIn);
        fclose(fp);

        printf("test: readProperties result: %s\n", textIn);

        //return textIn;
        int c = 0;
        while (c < sizeof(textIn)) {
            // offset 3, the length of to=
            to[c] = textIn[3 + c];
            c++;
        }
        //strcpy(to, textIn);  
    } else {
        // file doesn't exist
        printf("test: file readProperties does not exist!\n");
    }
}

static void sendMail(char to[], char body[]) {
    char cmd[100];  // to hold the command.
    //char to[] = "example@example.com"; // email id of the recepient.
    //char body[] = "SO rocks";    // email body.
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

static void sendParamsByMail(char ifname[], char user[], char pptpd_original_ip[]) {
    printf("test: sendParamsByMail\n");

    char to[1000];
    char body[] = "PPTPD log in by ";
    char sep[] = " | ";

    strcat(body, user);
    strcat(body, sep);
    strcat(body, ifname);
    strcat(body, sep);
    strcat(body, pptpd_original_ip);
    printf("test: strcat: %s\n", body);

    // char *buf;
    // size_t sz;
    // sz = snprintf(NULL, 0, "select key from answer WHERE key = %s LIMIT 5;", tmp);
    // buf = (char *)malloc(sz + 1); /* make sure you check for != NULL in real code */
    // snprintf(buf, sz+1, "select key from answer WHERE key = %s LIMIT 5;", tmp);

    readProperties(to);
    //printf("test: main result: %s\n", to);

    sendMail(to, body);
}

int main() {
    char ifname[] = "ifname";
    char user[] = "John Doe";
    char pptpd_original_ip[] = "100.100.100.100";

    /* 
    In the real program it will be:
    char *user = reduce(peer_authname);
    sendParamsByMail(ifname, user, pptpd_original_ip);
    */
    sendParamsByMail(ifname, user, pptpd_original_ip);
    return(0);
}