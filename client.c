/* CREATED BY GH0ST */
#include "globals.h"

#define true 1
#define false 0
#define SW 800
#define SH 400

// Size of pixel in pixels
#define SZPIX 1
// { -lessblurry ; +moreblurry }
#define ALIASBLUR  0.38f

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

/* NETWORKING */
#define BUFSIZE 1024
int sockfd, portno, n;
int serverlen;
struct sockaddr_in serveraddr;
struct hostent *server;
char *hostname;
char buf[BUFSIZE];

void error(char *msg) {
    perror(msg);
    exit(0);
}

void SendHandshakeToServer()
{
	bzero(buf, BUFSIZE);
	char* type = "HANDSH##";
	memcpy(buf, type, strlen(type));
	serverlen = sizeof(serveraddr);
		n = sendto(sockfd, buf, strlen(buf), 0, &serveraddr, serverlen);
	if (n < 0) 
		error("ERROR in sendto\r\n");
}



void PutPixel(int x, int y)
{
	glVertex2f(x*SZPIX - SZPIX/2, y*SZPIX-SZPIX/2);
}

float theta = 0.0f;
void display(void)
{

theta += 0.1f;
/* Send buf to server */
/*
memcpy(buf, &theta, sizeof(float));
serverlen = sizeof(serveraddr);
n = sendto(sockfd, buf, strlen(buf), 0, &serveraddr, serverlen);
if (n < 0) 
	error("ERROR in sendto\r\n");
*/

/* print the server's reply */
/*
    n = recvfrom(sockfd, buf, strlen(buf), 0, &serveraddr, &serverlen);
    if (n < 0) 
      error("ERROR in recvfrom");

    float thetaread = 0;
    memcpy(&thetaread, buf, sizeof(float));
    printf("Echo from server: %f\r\n", thetaread);
*/

/* clear window */
glClear(GL_COLOR_BUFFER_BIT);

/* Draw pixels */
glPointSize(SZPIX);
glColor4f(1, 1, 1, 1);
glBegin(GL_POINTS);
	PutPixel(10, 10);
glEnd();

/* flush GL buffers */
glFlush();
glutPostRedisplay();
}
void init()
{
	/* set clear color to black */
	glClearColor (0.0, 0.0, 0.0, 0.0);
	/* set fill color to white */
	glColor3f(1.0, 1.0, 1.0);
	/* set up standard orthogonal view with clipping */
	/* box as cube of side 2 centered at origin */
	/* This is default view and these statement could be removed */
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho(0, SW, 0, SH, -1.0, 1.0);

	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
int main(int argc, char** argv)
{
/* Check if server IP is entered */
if (argc < 2) 
{
	error("Enter server IP as argument.\r\n");	
	exit(1);
} 
hostname = argv[1];
portno = PORT;

/* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);

	SendHandshakeToServer();
	/* Check the server's response to handshake */
	n = recvfrom(sockfd, buf, strlen(buf), 0, &serveraddr, &serverlen);
    	if (n < 0) 
      		error("ERROR in recvfrom");

    	unsigned int thetaread = 0;
    	memcpy(&thetaread, buf, sizeof(unsigned int));
	if (thetaread == 2637) { printf("Connection established.\r\n"); }

/* Initialize mode and open a window in upper left corner of screen */
/* Window title is name of program (arg[0]) */
glutInit(&argc,argv);
glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
glutInitWindowSize(SW,SH);
glutInitWindowPosition(0,0);
glutCreateWindow("simple");
glutDisplayFunc(display);
init();
glutMainLoop();
}
