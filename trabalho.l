%{
#include <stdio.h>
#include <string.h>
#include "auxiliar.h"

char *lat, *lon, *name, *desc, *ele, *time, *speed;
int numPt = 0;

LWpt wpt;
Trilho trlh;
LPontos pts;

%}

%x GPX
%x WAYPOINT
%x PONTO
%x TRILHO


%%
"<gpx" 											{mostraInicio(); BEGIN INITIAL;}

"<wpt" {BEGIN WAYPOINT;}
<WAYPOINT>("lat="[\"\']\-?[0-9 .]+[\"\'])       {yytext[yyleng-1]='\0'; wpt.lat = strdup(yytext+5);}
<WAYPOINT>("lon="[\"\']\-?[0-9 .]+[\"\'])       {yytext[yyleng-1]='\0'; wpt.lon = strdup(yytext+5);}
<WAYPOINT>("<name>"[! #-~ ]*"</name>")          {yytext[yyleng-7]='\0'; wpt.name = strdup(yytext+6);}
<WAYPOINT>("<desc>"[! #-~ ]*"</desc>")          {yytext[yyleng-7]='\0'; wpt.desc = strdup(yytext+6);}
<WAYPOINT>("</wpt>")							{mostraWaypoint(wpt); wpt.name = NULL; wpt.lat = NULL; wpt.lon = NULL; BEGIN INITIAL;}

"<trk>" 										{BEGIN TRILHO;}
<TRILHO>("<name>"[! #-~ ]*"</name>")  			{yytext[yyleng-7]='\0'; trlh.name = strdup(yytext+6);}
<TRILHO>("<desc>"[! #-~ ]*"</desc>")  			{yytext[yyleng-7]='\0'; trlh.desc = strdup(yytext+6);}
<TRILHO>("<trkpt")                   			{mostraTrilho(trlh,numPt); numPt++; BEGIN PONTO;}

"<trkpt"								        {BEGIN PONTO;}
<PONTO>("lat="[\"\']\-?[0-9 .]+[\"\'])          {yytext[yyleng-1]='\0'; lat = strdup(yytext+5);}
<PONTO>("lon="[\"\']\-?[0-9 .]+[\"\'])          {yytext[yyleng-1]='\0'; lon = strdup(yytext+5);}
<PONTO>("<ele>"\-?[0-9 .]+"</ele>")             {yytext[yyleng-6]='\0'; ele = strdup(yytext+5);}
<PONTO>("<time>"[! #-~ ]*"</time>")             {yytext[yyleng-7]='\0'; time = strdup(yytext+6);}
<PONTO>("</trkpt>")                	            {if(ele==NULL) {ele = strdup("0");}; pts = insPonto(lat,lon,ele,time,pts); BEGIN INITIAL;}

"</trkseg>" 									{mostraPontos(pts); mostraCaminho(pts); pts = NULL;}


"</gpx>"                       				    {mostraFim();}
<INITIAL,GPX,WAYPOINT,TRILHO,PONTO>([ \n\t]*|.) {;}

%%

int yywrap() {
	return 1;
}

int main() {
	yylex();
	return 0;
}
