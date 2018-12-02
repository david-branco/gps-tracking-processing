/* Cabeçalhos */
int mostraInicio(){
  FILE *f;
  f = fopen("output.kml","w+");
  fprintf(f,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  fprintf(f,"<kml xmlns=\"http://www.opengis.net/kml/2.2\"\n\t xmlns:gx=\"http://www.google.com/kml/ext/2.2\">\n");
  fprintf(f,"  <Document>\n    <name>GPS device</name>\n"); 
  fprintf(f,"<!-- Normal waypoint style -->\n    <Style id=\"waypoint_n\">\n      <IconStyle>\n\t<Icon>\n          <href>http://maps.google.com/mapfiles/kml/pal4/icon61.png</href>\n\t</Icon>\n      </IconStyle>\n    </Style>\n");
  fprintf(f,"<!-- Highlighted waypoint style -->\n    <Style id=\"waypoint_h\">\n      <IconStyle>\n\t<scale>1.2</scale>\n\t<Icon>\n          <href>http://maps.google.com/mapfiles/kml/pal4/icon61.png</href>\n\t</Icon>\n      </IconStyle>\n    </Style>\n");
  fprintf(f,"    <StyleMap id=\"waypoint\">\n      <Pair>\n\t<key>normal</key>\n\t<styleUrl>#waypoint_n</styleUrl>\n      </Pair>\n      <Pair>\n\t<key>highlight</key>\n\t<styleUrl>#waypoint_h</styleUrl>\n      </Pair>\n    </StyleMap>\n    <Folder>\n      <name>Waypoints</name>\n");
  fclose(f);
}

int mostraFim(){
  FILE *f;
  f = fopen("output.kml","a+");
  fprintf(f,"    </Folder>\n  </Document>\n</kml>");
  fclose(f);
}


/* Waypoint */
typedef struct sLWaypoints {
	char *lat;
	char *lon;
	char *name;
	char *desc;
}LWpt;


int mostraWaypoint(LWpt wpt){
	FILE *f;
    f = fopen("output.kml","a+");
	fprintf(f,"      <Placemark>\n");	
	
	if(wpt.name != NULL){ fprintf(f,"\t<name>%s</name>\n",wpt.name); }
	else{ fprintf(f,"\t<name>Ponto</name>\n"); }
		
	if(wpt.desc!=NULL){ fprintf(f,"\t<description>%s</description>\n",wpt.desc); }
	
	fprintf(f,"\t<styleUrl>#waypoint</styleUrl>\n");
	fprintf(f,"\t<Point>\n");
	fprintf(f,"\t <coordinates>%s,%s,0.000000</coordinates>\n",wpt.lon, wpt.lat);
	fprintf(f,"\t</Point>\n");
	fprintf(f,"      </Placemark>\n");
    fclose(f);
  
	return 1;
}

/* Trilho */

typedef struct sTrilho{
	char *name;
	char *desc;
} Trilho;

int mostraTrilho(Trilho trlh, int numPt){
	FILE *a;
    a = fopen("output.kml","a+");
	fprintf(a,"<Folder>\n\t<name>Track %d</name>\n",numPt);
	
	if(trlh.name != NULL) { fprintf(a,"\t<name>%s</name>\n",trlh.name); } 
	else { fprintf(a,"\t<name>Ponto</name>\n"); }
	
	fprintf(a,"\t<description>%s</description>\n\n",trlh.desc);
	fprintf(a,"<Folder>\n\t<name>Points</name>\n");
	fclose(a);

	return 1;
}

/* Ponto */

typedef struct sPonto{
	char *lat;
	char *lon;
	char *ele;
	char *time;
	struct sPonto *seg;
} Ponto, *LPontos;

LPontos insPonto(char *lat, char *lon, char *ele, char *time, LPontos pts) {
	LPontos aux = NULL, res = NULL;
	LPontos perc = pts;


	if(pts == NULL){
		pts =(LPontos) malloc(sizeof(Ponto));
		pts->lat = lat;
		pts->lon = lon;
		pts->ele = ele;
		pts->time = time;
		pts->seg = NULL;
	}

	else {
		while(perc->seg != NULL){
			perc = perc->seg;
		}
		aux = insPonto(lat, lon, ele, time, aux);
		perc->seg = aux;
	}
	return pts;
}

int mostraPontos(LPontos pts){
  LPontos aux = pts;
  int x = 0;
  
  FILE *f;
  f = fopen("output.kml","a+");
  
	while(aux != NULL){
	  fprintf(f,"\t<Placemark>\n\t<name>Point %d</name>",x);
	  fprintf(f,"\t<description><![CDATA[ <table> <tr> <td>Longitude: %s </td> </tr> <tr> <td>Latitude: %s </td> </tr> <tr> <td>Time: %s </td></tr></table> ]]> </description>\n",aux->lon,aux->lat,aux->time);
	  fprintf(f,"\t<TimeStamp>\n\t\t<when> %s </when>\n\t</TimeStamp>\n",aux->time);
	  fprintf(f,"\t<Point>\n");
	  fprintf(f,"\t\t<coordinates>%s,%s,%s</coordinates>\n",aux->lon, aux->lat, aux->ele);
	  fprintf(f,"\t</Point>\n");
	  fprintf(f,"</Placemark>\n\n");

	  x++;
	  aux = aux->seg;
	}
	
	fclose(f);

	return 1;
}

int mostraCaminho(LPontos pts){
  LPontos aux = pts;
  FILE *f;
  f = fopen("output.kml","a+");
  fprintf(f,"</Folder>\n\t<Placemark>\n\t<name>Path</name>");
  fprintf(f,"<styleUrl>#lineStyle</styleUrl>\n<LineString>\n<tessellate>1</tessellate>\n<coordinates>\n");
  
  while(aux != NULL){
  fprintf(f,"\t\t%s,%s,%s\n",aux->lon,aux->lat,aux->ele);
  aux = aux->seg;
  }

  fprintf(f,"</coordinates>\n</LineString>\n</Placemark>\n</Folder>");
  fclose(f);
}
