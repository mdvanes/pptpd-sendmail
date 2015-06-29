/*
 * $Id: pptpd-sendmail.c,v 0.1 2015/28/06 18:04:39 mdvanes Exp $
 * pptpd-sendmail.c - pppd plugin to send mail for a pptpd user
 *
 * 2015 M.D. van Es - based on pptpd-logwtmp by James Cameron.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 */
#include <unistd.h>
#include <utmp.h>
#include <stdio.h>
#include <string.h>
#include <pppd/pppd.h>

char pppd_version[] = VERSION;

static char pptpd_original_ip[PATH_MAX+1];
/* TODO remove pptpd_logwtmp_strip_domain */
static bool pptpd_logwtmp_strip_domain = 0;

static option_t options[] = {
  { "pptpd-original-ip",          /* name from pppd.h */
    o_string,                     /* type */
    pptpd_original_ip,            /* addr */
    "Original IP address of the PPTP connection", /* description */
    OPT_STATIC,                   /* flags */
    NULL,                         /* addr2 */
    PATH_MAX                      /* upper_limit */
  },
  { "pptpd-logwtmp-strip-domain", o_bool, &pptpd_logwtmp_strip_domain,
    "Strip domain from username before logging", OPT_PRIO | 1 },
  { NULL }
};

static char *reduce(char *user)
{
  char *sep;
  if (!pptpd_logwtmp_strip_domain) return user;

  sep = strstr(user, "//"); /* two slash */
  if (sep != NULL) user = sep + 2;
  sep = strstr(user, "\\"); /* or one backslash */
  if (sep != NULL) user = sep + 1;
  return user;
}

static char readProperties(char to[]) {
    //printf("test: readProperties\n");
    /* TODO read property file in the same dir with name pptpd-sendmail.properties */

    char textIn[1000];

    char fname[] = "pptpd-sendmail.properties";

    if( access( fname, F_OK ) != -1 ) {
        // file exists
        FILE *fp;
        fp=fopen(fname, "r");
        fscanf(fp, "%[^\n]\n", textIn);
        fclose(fp);

        //printf("test: readProperties result: %s\n", textIn);

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

    // printf("sendmail %s < %s\n\n",to,tempFile); // log command for debugging
    // printf("%s\n\n",body); // log body for debugging

    sprintf(cmd,"sendmail %s < %s",to,tempFile); // prepare command.
    system(cmd);     // execute it.
}

static void sendParamsByMail(char ifname[], char user[], char pptpd_original_ip[]) {
    //printf("test: sendParamsByMail\n");

    char to[1000];
    char body[] = "PPTPD log in by ";
    char sep[] = " | ";

    strcat(body, user);
    strcat(body, sep);
    strcat(body, ifname);
    strcat(body, sep);
    strcat(body, pptpd_original_ip);
    //printf("test: strcat: %s\n", body);

    // char *buf;
    // size_t sz;
    // sz = snprintf(NULL, 0, "select key from answer WHERE key = %s LIMIT 5;", tmp);
    // buf = (char *)malloc(sz + 1); /* make sure you check for != NULL in real code */
    // snprintf(buf, sz+1, "select key from answer WHERE key = %s LIMIT 5;", tmp);

    readProperties(to);
    //printf("test: main result: %s\n", to);

    sendMail(to, body);
}

static void ip_up(void *opaque, int arg)
{
  char *user = reduce(peer_authname);
  if (debug)
    /* notice - pppd.h; log a notice-level message */
    notice("pptpd-sendmail.so ip-up %s %s %s", ifname, user, 
       pptpd_original_ip);
  /*logwtmp(ifname, user, pptpd_original_ip);*/
  sendParamsByMail(ifname, user, pptpd_original_ip);
}

static void ip_down(void *opaque, int arg)
{
  if (debug) 
    /* notice - pppd.h; log a notice-level message */
    notice("pptpd-sendmail.so ip-down %s", ifname);
  /*logwtmp(ifname, "", "");*/
  sendParamsByMail(ifname, "logout", "logout");
}

void plugin_init(void)
{
  //printf("pptpd-sendmail: plugin_init\n");
  add_options(options);
  add_notifier(&ip_up_notifier, ip_up, NULL);
  add_notifier(&ip_down_notifier, ip_down, NULL);
  if (debug) 
    /* notice - pppd.h; log a notice-level message */
    notice("pptpd-sendmail: $Version$");
}
