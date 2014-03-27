//Librerias que usaremos en este programa
//MysqlClient lo usaremos para manejar la base de datos.
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>

//Estructuras a usar
  typedef struct{
    char nombre[100];
  }DatosCliente;

  typedef struct{
    char nombre[100];
    float costo;
    char fecha_inicio[45];
    char fecha_final[45];
  }DatosBien;

  typedef struct nodoBien{
    DatosBien datos; 
    int ID;
    struct nodoBien* sig;
  }nodoBien;

  typedef struct nodo{
    int id;
    DatosCliente datos;
    struct nodo *sig;
    struct nodo *ant;
    nodoBien *headBien; 
  }nodoCliente;

  typedef struct{
    nodoCliente *headClientes; 
  }ListaClientes;
//Funciones
  //Inicializar Lista
    /**Crea dinamicamente una Lista de clientes con base en el struct
       La inicializa creando el primer valor como Null**/
    ListaClientes* crearListaClientes( ListaClientes * l1 ){
      l1 = malloc(sizeof(ListaClientes));
      l1 -> headClientes = NULL;
      return l1;
    }
  //Insertar nodo cliente
    /**Aqui se empieza a llenar un nodo del Struct Lista Clientes**/
    void insertar_nodo_clientes(ListaClientes** lista,int llave, DatosCliente informacion){ 
      /**Crearemos variables del tipo struct nodo, y al auxiliar lo
          inicializaremos a semejanza del nodoCLiente**/
	    	nodoCliente *aux, *iterador;
	    	aux = malloc(sizeof(nodoCliente));

	    	if( aux == NULL ){
	      		return; 
	    	}
      /**Ahora le asignamos los valores que nos pasaron por referencia desde
        el inicio a nuestra variable auxiliar**/
	      	aux -> datos = informacion;
	      	aux -> id = llave;
	      	aux -> ant = NULL;
	      	aux -> sig = NULL;
	      	aux -> headBien = NULL;

      /**Si la el valor de lista headClientes es igual a NULL entonces igualar
        el valor con aux
        Si el valor de headCLientes id es igual a la llave dada, el valor
        siguiente del auxiliar será el actual HeadClientes, y  el valor
        del axiliar actual será igual al headClientes osea el anterior**/
      		if( (*lista) -> headClientes == NULL ){
	        	(*lista) -> headClientes = aux;
	        	return; 
      		}
      		if( (*lista) -> headClientes -> id == llave ){
        		aux -> sig = (*lista) -> headClientes;
        		(*lista) -> headClientes = aux;
        		return;
      		}

      		iterador = (*lista) -> headClientes;

      		while( iterador -> sig != NULL ){
        		iterador=iterador->sig;
      		}

		    aux -> ant = iterador;
		    iterador -> sig = aux;
		    return;
    }

  //Consultar Lista de Clientes
    ListaClientes* consulta(){
      MYSQL *conexion;
      MYSQL_RES *R;
      MYSQL_ROW COL;
      char *servidor = "localhost";
      char *usuario = "root";
      char *pass = "1234";
      char *base = "casaempenio";
      conexion = mysql_init(NULL);
      mysql_real_connect(conexion,servidor,usuario,pass,base,0,NULL,0);
      char sentencia[100]="SELECT * from clientes ;";
      char buffer[512];
      mysql_query(conexion,sentencia);
      R = mysql_use_result(conexion);
      ListaClientes* lista1 = NULL;
      lista1 = crearListaClientes(lista1);
      if(R!=NULL){
      	COL = mysql_fetch_row(R);
	    while (COL != NULL){
	        int id;
	        char *ptr,*ptr2;
	        DatosCliente info;
	        ptr = COL[0];
	        ptr2 = COL[1];
	        id = atoi(ptr);
	        strcpy(info.nombre,ptr2);
	        insertar_nodo_clientes(&lista1,id,info);
	        COL = mysql_fetch_row(R);
	    }
      }else{
        puts("ADVERTENCIA: La base de datos se encuentra vacia"); 
      }
      mysql_close(conexion);
      return lista1;
    }
  //Imprimir la Lista de Clientes
    void imprimir_lista( ListaClientes *lista ){
      nodoCliente *iterador;
      iterador = lista -> headClientes;
      if( iterador == NULL ){
        puts("ADVERTENCIA: La base de datos se encuentra vacia");
      }
      /**Mientras el iterador no tenga el valor Nulo, pediremos el id y nombre**/
      while( iterador != NULL ){
        printf("%d      ",iterador->id);
        puts((iterador->datos).nombre);
        iterador = iterador -> sig;
      }
    }
  //Insertar nuevo cliente
    void insertar_nvo_cliente(char*nombre){
      MYSQL *conexion;
      MYSQL_RES *R;
      MYSQL_ROW COL;
      char *servidor = "localhost";
      char *usuario = "root";
      char *pass = "1234";
      char *base = "casaempenio";

      conexion = mysql_init(NULL);
      mysql_real_connect(conexion,servidor,usuario,pass,base,0,NULL,0);
      char *sentencia="insert into clientes(nombre) values(";
      char buffer[500];
      sprintf(buffer,"%s'%s'); ",sentencia,nombre);
      mysql_query(conexion,buffer);
      mysql_close(conexion);
    }
  //Eliminar un cliente
    void eliminar_cliente(char*nombre){
      MYSQL *conexion;
      MYSQL_RES *R;
      MYSQL_ROW COL;
      char *servidor = "localhost";
      char *usuario = "root";
      char *pass = "1234";
      char *base = "casaempenio";
      conexion = mysql_init(NULL);
      mysql_real_connect(conexion,servidor,usuario,pass,base,0,NULL,0);
      char *sentencia="DELETE FROM clientes where nombre=";
      char buffer[500];
      sprintf(buffer,"%s'%s'; ",sentencia,nombre);
      mysql_query(conexion,buffer);
      mysql_close(conexion);
    }
  //Modificar un cliente
    void modificar_cliente(char*nombre, char*nombre2){
      MYSQL *conexion;
      MYSQL_RES *R;
      MYSQL_ROW COL;
      char *servidor = "localhost";
      char *usuario = "root";
      char *pass = "1234";
      char *base = "casaempenio";
      conexion = mysql_init(NULL);
      mysql_real_connect(conexion,servidor,usuario,pass,base,0,NULL,0);
      char *sentencia = "update clientes set nombre=";
      char *sentencia2 = "WHERE nombre=";
      char buffer[500];
      sprintf(buffer,"%s '%s' %s '%s';",sentencia,nombre,sentencia2,nombre2);
      mysql_query(conexion,buffer);
      mysql_close(conexion);
    }
  //Agregar cliente a Lista de Clientes
    void agregar_cliente_lista(ListaClientes **l1, DatosCliente info_nueva){
      MYSQL *conexion;
      MYSQL_RES *R;
      MYSQL_ROW COL;
      char *servidor = "localhost";
      char *usuario = "root";
      char *pass = "1234";
      char *base = "casaempenio";
      conexion = mysql_init(NULL);
      mysql_real_connect(conexion,servidor,usuario,pass,base,0,NULL,0);
      char *sentencia="SELECT * from clientes where nombre=";
      char buffer[500];
      sprintf(buffer,"%s '%s' ;",sentencia,(info_nueva.nombre));
      mysql_query(conexion,buffer);
      R= mysql_use_result(conexion);
      if(R!=NULL){
        COL = mysql_fetch_row(R);
        while (COL != NULL){
          int id;
          char *ptr,*ptr2;
          DatosCliente info;
          ptr=COL[0];
          ptr2=COL[1];
          id=atoi(ptr);
          strcpy(info.nombre,ptr2);
          insertar_nodo_clientes(l1,id,info);
          COL = mysql_fetch_row(R);
        }
      }else{
        puts("El dato solicitado no se pudo ingresar en la base de datos"); 
      }
      mysql_close(conexion);
    }
  //Buscar a un cliente en la Lista de Clientes
    int buscar_en_lista(ListaClientes **l1, char* nombre){
      nodoCliente* iterador=(*l1)->headClientes;  
      int i=0;
      while(iterador!=NULL){
        if(strcmp((iterador->datos.nombre),nombre)==0){
          i=1;
          break;
        }
        iterador=iterador->sig;
      }
      return i;
    }
  //Eliminación por ID
    void eliminar_cliente_id(int id){
      MYSQL *conexion;
      MYSQL_RES *R;
      MYSQL_ROW COL;
      char *servidor = "localhost";
      char *usuario = "root";
      char *pass = "1234";
      char *base = "casaempenio";
      conexion = mysql_init(NULL);
      mysql_real_connect(conexion,servidor,usuario,pass,base,0,NULL,0);
      char sentencia[100]="delete from clientes where idclientes=";
      char buffer[512]; 
      sprintf(buffer,"%s %d;",sentencia,id);
      mysql_query(conexion,buffer);
	mysql_close(conexion);
    }
  //Obtener un ID
    int get_id(char*nombre,ListaClientes *lista){
      nodoCliente *iterador=lista->headClientes;
      while(iterador!=NULL){
        if(strcmp((iterador->datos.nombre),(nombre))==0){
          break;    
        }
        iterador=iterador->sig;
      }
      return (iterador->id);
    }
  //Modificar un registro en la Lista de Clientes
    void modificar_en_lista(ListaClientes **l1, DatosCliente info){
      nodoCliente* iterador=(*l1)->headClientes;
      while(iterador!=NULL){
        if(strcmp((iterador->datos.nombre),(info.nombre))==0){
          break;
        }
        iterador=iterador->sig;
      }
      if(iterador!=NULL){
        char *nvo_nombre;
        puts("Escribe el nuevo nombre");
        setbuf(stdin,NULL);
        gets(nvo_nombre);
        while(strcmp(nvo_nombre,"")==0){
          puts("Escriba un nombre valido");
          setbuf(stdin,NULL);
          gets(nvo_nombre);   
        }
        if(buscar_en_lista(l1,nvo_nombre)==0){
          modificar_cliente(nvo_nombre, iterador->datos.nombre);
          strcpy(iterador->datos.nombre,nvo_nombre);
        }else{
          puts("El cliente ya existe");
        }
      }else{
        puts("El cliente no existe"); 
      }
    }
  //Insertar nuevo bien
    void insertar_nvo_bien(char*nombre, char*cliente, float monto, char *fecha_ini, char *fecha_fin){
      MYSQL *conexion;
      MYSQL_RES *R;
      MYSQL_ROW COL;
      char *servidor = "localhost";
      char *usuario = "root";
      char *pass = "1234";
      char *base = "casaempenio";

      conexion = mysql_init(NULL);
      mysql_real_connect(conexion,servidor,usuario,pass,base,0,NULL,0);
      char *sentencia="insert into bienes(nombreBien,nombreCliente,monto,fecha_inicio,fecha_final) values(";
      char buffer[500];
      sprintf(buffer,"%s'%s','%s',%f,'%s','%s'); ",sentencia,nombre,cliente,monto,fecha_ini,fecha_fin);
      mysql_query(conexion,buffer);
      mysql_close(conexion);
    }
  //Insetart un Nodo de BIEN
    void insertar_bien(nodoCliente** cliente, int llave,DatosBien info){
      nodoBien *aux, *iterador;
      aux=malloc(sizeof(nodoBien));
      if(aux==NULL){
        return; 
      }
      aux->datos=info;
      aux->ID=llave;
      aux->sig=NULL;

      if((*cliente)->headBien==NULL){
        (*cliente)->headBien=aux;
        return; 
      }
      if((*cliente)->headBien->ID==llave){
        aux->sig=(*cliente)->headBien;
        (*cliente)->headBien=aux;
      return; 
      }

      iterador=(*cliente)->headBien;

      while(iterador->sig!=NULL){
        iterador=iterador->sig;   
      }

      iterador->sig=aux;
      return;
    }
  //Consulta de los BIENES
    nodoCliente* cargar_bienes(ListaClientes **lista,char *cliente){
      nodoCliente* iterador=(*lista)->headClientes;
      while(iterador!=NULL){
        if(strcmp((iterador->datos.nombre),(cliente))==0){
          break;
        }
        iterador=iterador->sig;
      }
      MYSQL *conexion;
      MYSQL_RES *R;
      MYSQL_ROW COL;
      char *servidor = "localhost";
      char *usuario = "root";
      char *pass = "1234";
      char *base = "casaempenio";
      conexion = mysql_init(NULL);
      mysql_real_connect(conexion,servidor,usuario,pass,base,0,NULL,0);
      char sentencia[100]="SELECT * from bienes where nombreCliente=";
      char buffer[512]; 
      sprintf(buffer,"%s '%s';",sentencia,cliente);

      mysql_query(conexion,buffer);
      R= mysql_use_result(conexion);
      if(R!=NULL){
        COL = mysql_fetch_row(R);
        while (COL != NULL){
          int id;
          char *ptr,*ptr2,*ptr3,*ptr4,*ptr5;
          DatosBien info;
          ptr=COL[0];
          ptr2=COL[1];
          ptr3=COL[3];
          ptr4=COL[4];
          ptr5=COL[5];
          id=atoi(ptr);
          strcpy(info.nombre,ptr2);
          info.costo=atof(ptr3);
          strcpy(info.fecha_inicio,ptr4);
          strcpy(info.fecha_final,ptr5);  
          insertar_bien(&iterador,id,info);
          COL = mysql_fetch_row(R);
        }
      }else{
        puts("El cliente no tiene ningun bien registrado"); 
      }
	mysql_close(conexion);
      return iterador;
    }
  //Imprimir los BIENES
    void imprimir_bienes(nodoCliente* cliente){
      nodoBien *iterador= cliente->headBien;
      char buffer[600];
      while(iterador!=NULL){
        sprintf(buffer,"%s         %s             %s           %s",iterador->datos.nombre,cliente->datos.nombre,iterador->datos.fecha_inicio,iterador->datos.fecha_final);
        puts("IDProducto   Monto   Producto   Cliente      Fecha_inicio   Fecha_fin");
        printf(" %d           %f      ",iterador->ID,iterador->datos.costo);
        puts(buffer);
        iterador=iterador->sig;
      }
    }
  //Eliminar nodo de la Lista
    ListaClientes* eliminar_nodo_lista(ListaClientes* lista, int id){
      nodoCliente* aux1, *aux2;
      aux1=lista->headClientes;
      while(aux1->sig!=NULL && (aux1->sig->id)<id){
        aux1=aux1->sig;
      }
      aux2=lista->headClientes;
      while(aux2->sig!=NULL && (aux1->id)<=id){
        aux2=aux2->sig;
      }
      if(aux1->id==aux2->id){
        puts("La lista quedo vacia");
        return NULL;  
      }
      if(aux1->id==id){
        aux1=aux1->sig;
        aux1->ant=NULL;
        lista->headClientes=aux1;
        return lista;
      }
      if(aux2->id==id){
        aux1->sig=NULL;
        while(aux1->ant!=NULL){
          aux1=aux1->ant;
        }
        lista->headClientes=aux1; 
        return lista;
      }
      aux1->sig=aux2;
      aux2->ant=aux1;
      while(aux1->ant!=NULL){
        aux1=aux1->ant;
      }
      lista->headClientes=aux1; 
      return lista;
    }

int buscar_bien_lista(nodoCliente *cliente, int llave){
	nodoBien *iterador=cliente->headBien;	
	int i=0;
      	while(iterador!=NULL){
        if(iterador->ID==llave){
		char buffer[600];
		sprintf(buffer,"%s         %s             %s           %s",iterador->datos.nombre,cliente->datos.nombre,iterador->datos.fecha_inicio,iterador->datos.fecha_final);
		puts("IDProducto   Monto   Producto   Cliente      Fecha_inicio   Fecha_fin");
		printf(" %d           %f      ",iterador->ID,iterador->datos.costo);
		puts(buffer);
		  i=1;
          	break;
        }
        iterador=iterador->sig;
      }
      return i;
}

DatosBien obtener_datos_bien(nodoCliente *cliente, int llave){
	nodoBien *iterador=cliente->headBien;
	DatosBien info;
	while(iterador!=NULL){
		if(iterador->ID==llave){
			strcpy(info.nombre,iterador->datos.nombre);
			info.costo=iterador->datos.costo;
			strcpy(info.fecha_inicio,iterador->datos.fecha_inicio);	
			strcpy(info.fecha_final,iterador->datos.fecha_final);
					
		}
		iterador=iterador->sig;
	}
	return info;
}

void modificar_bien_nombre(char *nombre,char* nvo_nombre){
	MYSQL *conexion;
      MYSQL_RES *R;
      MYSQL_ROW COL;
      char *servidor = "localhost";
      char *usuario = "root";
      char *pass = "1234";
      char *base = "casaempenio";
      conexion = mysql_init(NULL);
      mysql_real_connect(conexion,servidor,usuario,pass,base,0,NULL,0);
	
      char *sentencia="update bienes set nombreBien=";
      char *sentencia2="where nombreBien=";
      char buffer[600]; 
      sprintf(buffer,"%s '%s' %s '%s';",sentencia,nvo_nombre,sentencia2,nombre);

      mysql_query(conexion,buffer);
	mysql_close(conexion);

}

void modificar_bien_monto(float monto,float nvo_monto){
	MYSQL *conexion;
      MYSQL_RES *R;
      MYSQL_ROW COL;
      char *servidor = "localhost";
      char *usuario = "root";
      char *pass = "1234";
      char *base = "casaempenio";
      conexion = mysql_init(NULL);
      mysql_real_connect(conexion,servidor,usuario,pass,base,0,NULL,0);
	
      char *sentencia="update bienes set monto=";
      char *sentencia2="where monto=";
      char buffer[600]; 
      sprintf(buffer,"%s %f %s %f;",sentencia,nvo_monto,sentencia2,monto);

      mysql_query(conexion,buffer);
	mysql_close(conexion);

}

void modificar_bien_fecha_inicio(char *fecha_inicio,char* nvo_fecha_inicio){
	MYSQL *conexion;
      MYSQL_RES *R;
      MYSQL_ROW COL;
      char *servidor = "localhost";
      char *usuario = "root";
      char *pass = "1234";
      char *base = "casaempenio";
      conexion = mysql_init(NULL);
      mysql_real_connect(conexion,servidor,usuario,pass,base,0,NULL,0);
	
      char *sentencia="update bienes set fecha_inicio=";
      char *sentencia2="where fecha_inicio=";
      char buffer[600]; 
      sprintf(buffer,"%s '%s' %s '%s';",sentencia,nvo_fecha_inicio,sentencia2,fecha_inicio);

      mysql_query(conexion,buffer);
	mysql_close(conexion);

}

void modificar_bien_fecha_final(char *fecha_final,char* nvo_fecha_final){
	MYSQL *conexion;
      MYSQL_RES *R;
      MYSQL_ROW COL;
      char *servidor = "localhost";
      char *usuario = "root";
      char *pass = "1234";
      char *base = "casaempenio";
      conexion = mysql_init(NULL);
      mysql_real_connect(conexion,servidor,usuario,pass,base,0,NULL,0);
	
      char *sentencia="update bienes set fecha_final=";
      char *sentencia2="where fecha_final=";
      char buffer[600]; 
      sprintf(buffer,"%s '%s' %s '%s';",sentencia,nvo_fecha_final,sentencia2,fecha_final);

      mysql_query(conexion,buffer);
	mysql_close(conexion);
}

void EliminarNodoBien( nodoCliente **nodocliente , int id){
        nodoBien *iterador = (*nodocliente) -> headBien;
	if(iterador->ID==id){
		(*nodocliente)->headBien=iterador->sig;
		free(iterador);
		return;
	}        
	while(iterador -> sig != NULL && iterador->ID < id){
          iterador = iterador -> sig;
        }
	
        if(iterador -> sig  == NULL){
          iterador = NULL;
		return;
        }

          iterador -> sig = iterador -> sig -> sig;
        
      }

 void eliminar_bien_id(int id){
      MYSQL *conexion;
      MYSQL_RES *R;
      MYSQL_ROW COL;
      char *servidor = "localhost";
      char *usuario = "root";
      char *pass = "1234";
      char *base = "casaempenio";
      conexion = mysql_init(NULL);
      mysql_real_connect(conexion,servidor,usuario,pass,base,0,NULL,0);
      char sentencia[100]="delete from bienes where idbienes=";
      char buffer[512]; 
      sprintf(buffer,"%s %d;",sentencia,id);
      mysql_query(conexion,buffer);
      mysql_close(conexion);
    }
int get_id_bien(DatosBien info,nodoCliente *cliente){
	MYSQL *conexion;
      MYSQL_RES *R;
      MYSQL_ROW COL;
      char *servidor = "localhost";
      char *usuario = "root";
      char *pass = "1234";
      char *base = "casaempenio";
      conexion = mysql_init(NULL);
      mysql_real_connect(conexion,servidor,usuario,pass,base,0,NULL,0);
      char sentencia[100]="select* from bienes where";
      char buffer[600];
      sprintf(buffer,"%s nombreBien='%s' and nombreCliente='%s' and fecha_inicio='%s' and fecha_final='%s';",sentencia,info.nombre,cliente->datos.nombre,info.fecha_inicio,info.fecha_final); 
	
	puts(buffer);
	mysql_query(conexion,buffer);
	R= mysql_use_result(conexion);
	int id=0;
	if(R!=NULL){
		COL = mysql_fetch_row(R);
		while (COL != NULL){
		  
		  char *ptr;
		  ptr=COL[0];
		  id=atoi(ptr);
		 COL = mysql_fetch_row(R);
		}
	}
      mysql_close(conexion);
	return id;

}

//Main
  int main(){
    int val;
    ListaClientes *Lista;
    Lista=consulta();
    system("clear");
    do{
      puts("Bienvenido al Sistema de Bienes. ¿Que desea realizar? ");
      puts("Escriba 1 para Mostrar la lista de los clientes.");
      puts("Escriba 2 para Dar de alta un cliente.");
      puts("Escriba 3 para Dar de baja un cliente.");
      puts("Escriba 4 para Modificar datos un cliente");    
      puts("Escriba 5 para Ver los bienes de un cliente.");
      puts("Escriba 6 para salir.");
      scanf("%d",&val);
      if(val==1){
        system("clear");
        imprimir_lista(Lista);
      }
      if(val==2){
        system("clear");
        char *ptr;
        char nombre1[100];
        DatosCliente dato_nuevo;  
        puts("Escribe el nombre del nuevo cliente");
        setbuf(stdin,NULL);
        gets(nombre1);
        strcpy(dato_nuevo.nombre,nombre1);
        ptr=dato_nuevo.nombre;
        insertar_nvo_cliente(ptr);
        agregar_cliente_lista(&Lista, dato_nuevo);
        imprimir_lista(Lista);
      }
      if(val==3){
        system("clear");
        DatosCliente dato;
        puts("Escribe el nombre a eliminar");
        setbuf(stdin,NULL);
        gets(dato.nombre);
        while(strcmp(dato.nombre,"")==0){
          puts("Escriba un nombre valido");
          setbuf(stdin,NULL);
          gets(dato.nombre);    
        }
        if(buscar_en_lista(&Lista,dato.nombre)==1){
          eliminar_cliente_id(get_id(dato.nombre,Lista));
          Lista=eliminar_nodo_lista(Lista,(get_id(dato.nombre,Lista)));
          imprimir_lista(Lista);
        }else{
          puts("El cliente no existe");     
        }
      }
      if(val==4){
        system("clear");
        DatosCliente dato;
        puts("Escribe el nombre a modificar");
        setbuf(stdin,NULL);
        gets(dato.nombre);
        modificar_en_lista(&Lista,dato);
        imprimir_lista(Lista);
        
      }
      if(val==5){
        system("clear");
        DatosCliente dato;
        int net;
        puts("Escribe el nombre del cliente");
        setbuf(stdin,NULL);
        gets(dato.nombre);
        puts("Escribe tu NETKEY");
        scanf("%d", &net);
        if(buscar_en_lista(&Lista,dato.nombre)==1){
          int val2;
          system("clear");
          nodoCliente* cliente = cargar_bienes(&Lista , dato.nombre);
          
            do{
          	
            printf("Bienvenido al submenu de bienes del cliente ");
            puts(dato.nombre);
            puts("Escriba 1 para Ver bienes del cliente.");
            puts("Escriba 2 para Ingresar nuevo bien.");
            puts("Escriba 3 para Modificar un bien.");
            puts("Escriba 4 para Eliminar bien.");    
            puts("Escriba 5 para regresar al menu principal.");
            setbuf(stdin,NULL);
            scanf("%d",&val2);
            if(val2==1){
		system("clear");
              imprimir_bienes(cliente);
            }
            if(val2==2){
              system("clear");
              char nombre[100] , *cliente2, fecha_ini[100],fecha_fin[100];
              float monto;
              DatosBien datosBien;
              cliente2=dato.nombre;
              puts("Escribe el nombre del nuevo bien");
              setbuf(stdin,NULL);
              gets(nombre);
              strcpy(datosBien.nombre,nombre);
	      setbuf(stdin,NULL);
              puts("Escribe el costo");
              scanf("%f" , &monto);
              datosBien.costo = monto;
              puts("Escribe la fecha de inicio");
              setbuf(stdin,NULL);
              gets(fecha_ini);
              strcpy(datosBien.fecha_inicio,fecha_ini);
              puts("Escribe el vencimiento");
              setbuf(stdin,NULL);
              gets(fecha_fin);
              strcpy(datosBien.fecha_final,fecha_fin);
              insertar_nvo_bien(nombre,cliente2,monto,fecha_ini,fecha_fin);
		int id=get_id_bien(datosBien,cliente);
		printf("%d\n",id);
              insertar_bien(&cliente,id,datosBien);
		puts("hola");
	      system("clear");
	      imprimir_bienes(cliente);
            }
            if(val2==3){
            	system("clear");
		int llave;
		puts("indica el id del producto que quieres modificar");
		setbuf(stdin,NULL);
		scanf("%d",&llave);
		if(buscar_bien_lista(cliente,llave)==1){
				int val3;
				DatosBien informacion=obtener_datos_bien(cliente,llave);
				puts("Desea cambiar el nombre del producto. 1 para si");
				setbuf(stdin,NULL);			
				scanf("%d",&val3);
				if(val3==1){
					char *nvo_nombre;
					puts("Escriba el nuevo nombre del producto");
					setbuf(stdin,NULL);					
					gets(nvo_nombre);
					
					modificar_bien_nombre(informacion.nombre,nvo_nombre);
				}
				puts("Desea cambiar el monto del producto. 1 para si");
				setbuf(stdin,NULL);			
				scanf("%d",&val3);
				if(val3==1){
					float monto;
					puts("Escriba el nuevo nombre del producto");
					setbuf(stdin,NULL);					
					scanf("%f",&monto);
					setbuf(stdin,NULL);
					modificar_bien_monto(informacion.costo,monto);
				}
				puts("Desea cambiar la fecha de inicio del producto. 1 para si");
				setbuf(stdin,NULL);			
				scanf("%d",&val3);
				if(val3==1){
					char *nvo_fecha_inicio;
					puts("Escriba la nueva fecha de inicio del producto");
					setbuf(stdin,NULL);					
					gets(nvo_fecha_inicio);
					
				modificar_bien_fecha_inicio(informacion.fecha_inicio,nvo_fecha_inicio);
				}
				puts("Desea cambiar la fecha final del producto. 1 para si");
				setbuf(stdin,NULL);			
				scanf("%d",&val3);
				if(val3==1){
					char *nvo_fecha_final;
					puts("Escriba la nueva fecha de inicio del producto");
					setbuf(stdin,NULL);					
					gets(nvo_fecha_final);
					
				modificar_bien_fecha_final(informacion.fecha_final,nvo_fecha_final);
				}
				
				cliente->headBien=NULL;
				cliente=cargar_bienes(&Lista , dato.nombre);
			
		}else{
			puts("El bien no existe");		
		}

            }
            if(val2==4){
		int id;
			  system("clear");
              puts("Hola Escribe el ID del bien que quieres eliminar");
              setbuf(stdin,NULL);
              scanf("%d" , &id);
              eliminar_bien_id(id);
	      cliente->headBien=NULL;
				cliente=cargar_bienes(&Lista , dato.nombre);

            }

          }while(val2!=5);
            Lista->headClientes->headBien=NULL;
            
        }else{
          puts("El cliente no existe");
        }
      }
    }while(val!=6);
  }
