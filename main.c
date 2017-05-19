#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/shm.h>

struct Vagon{
	int pasajeros;
	bool sem;
};

struct Estacion{
   char *nombre;
   int pasajeros;
   Vagon vags[2];
};

struct Bus
{   int placa;
	int pasajeros; 
	int capacidad;
	int paradas;
	Estacion* ruta[20];
};
struct Transmilenio
{

};

Estacion* crearEstacion (char *name);
Bus crearBus(int id,int cap,Estacion* r[],int tam);

int main ()
{	
	
	pid_t pid ;
	//bool* finPrograma=new bool();
	//*finPrograma=false;
	key_t Clave;
	int Id_Memoria;
	bool *Memoria = NULL;
	Clave = ftok ("/bin/ls", 33);
	Id_Memoria = shmget (Clave, sizeof(bool)*2, 0777| IPC_CREAT);
	Memoria = (bool *)shmat (Id_Memoria, (char *)0, 0);
	Estacion* calle45=crearEstacion("calle45");
	Estacion* calle53=crearEstacion("calle53");
	Estacion* calle57=crearEstacion("calle57");
	Estacion* calle63=crearEstacion("calle63");
	Estacion* calle72=crearEstacion("calle72");
	Estacion* expreso[3]={calle45,calle53,calle72};
	Bus a1= crearBus(1234,200,expreso,3);
	if ((pid=fork())==0){
		//printf(" Si creo ");
		bool finprograma=false; 
		key_t Clave;
		int Id_Memoria;
		bool *Memoria = NULL;
		Clave = ftok ("/bin/ls", 33);
		Id_Memoria = shmget (Clave, sizeof(bool)*2, 0777);
		Memoria = (bool *)shmat (Id_Memoria, (char *)0, 0);
		Memoria[0]=finprograma;
		printf("Entonces %d \n",Memoria[0]);
			if (Memoria[0]==false){
				finprograma=true;
				Memoria[0]=finprograma;
				for (int j = 0; j < 300; j++)
				{
					/* code */
					printf(" %d ",j);
				}
				printf(" acabe ");
				finprograma=false;
				Memoria[0]=finprograma;
			}
		printf("Entonces %d \n",Memoria[0]);
		// while (Memoria[0]==false){	
		//	srand(time(NULL));
		//	int random=rand()%10; //random entre 0 y 9 para el sleep
		//	usleep(100000*random);
		//	printf("FIN");
		//	finPrograma=true;
		//	printf("%d",finPrograma);
		//  }
		shmdt ((char *)Memoria);
	}else{
		//printf(" No creo ");
		//printf(" Entramos");
		key_t Clave;
		int Id_Memoria;
		bool *Memoria = NULL;
		Clave = ftok ("/bin/ls", 33);
		Id_Memoria = shmget (Clave, sizeof(bool)*2, 0777);
		Memoria = (bool *)shmat (Id_Memoria, (char *)0, 0);
				//while (*finPrograma==false)
		printf("Hola %d \n",Memoria[0]); 
		  while (Memoria[0]==true){
			//printf("%d",Memoria[0]);
		  }
		  if (Memoria[0]==false){
				bool finprograma=true;
				Memoria[0]=finprograma;
				for (int j = 300; j < 600; j++)
				{
					/* code */
					printf(" %d ",j);
				}
				printf(" acabe ");
				finprograma=false;
				Memoria[0]=finprograma;
			}

		  shmdt ((char *)Memoria);
		  printf(" salio ");
	}
	/*if ((pid=fork())==0){
		printf(" Entramos");
		/*key_t Clave;
		int Id_Memoria;
		bool *Memoria = NULL;
		Clave = ftok ("/bin/ls", 33);
		Id_Memoria = shmget (Clave, sizeof(bool)*2, 0777);
		Memoria = (bool *)shmat (Id_Memoria, (char *)0, 0);
				//while (*finPrograma==false)
			//printf("Hola %d \n",Memoria[0]); 
		  while (Memoria[0]==false){
			printf("%d",Memoria[0]);
		  }
		  shmdt ((char *)Memoria);
	}
	else{

			printf("No entramos ");
	}*/
	shmdt ((char *)Memoria);
	shmctl (Id_Memoria, IPC_RMID, (struct shmid_ds *)NULL);
	return 0;

}

Estacion* crearEstacion (char *name){
	Estacion* p=new Estacion();
		p->nombre=name;
	Vagon p1;
		p1.pasajeros=0;
		p1.sem=false;
	Vagon p2;
		p2.pasajeros=0;
		p2.sem=false;
		p->vags[0]=p1;
		p->vags[1]=p2;
	return p;
}

Bus crearBus(int id,int cap,Estacion* r[],int tam){
	Bus a1;
	a1.placa=id;
		a1.pasajeros=0;
		a1.capacidad=cap;
		a1.paradas=tam;
	for (int i=0;i<tam;i++){
			a1.ruta[i]=r[i];
	}
	return a1;
}

void entrarAEstacion(Estacion* x){
	while ((x->vags[0].sem)){

	}
	if (!(x->vags[0].sem)){
		x->vags[0].sem=true;
		int pj=x->vags[0].pasajeros;
		x->vags[0].pasajeros=pj+2;
		x->vags[0].sem=false;
	}
	while ((x->vags[1].sem)){

	}
	if (!(x->vags[1].sem)){
		x->vags[1].sem=true;
		int pj=x->vags[1].pasajeros;
		x->vags[1].pasajeros=pj+2;
		x->vags[1].sem=false;
	}
}
void salirEstacion(Estacion* x){
	while ((x->vags[0].sem)){

	}
	if (!(x->vags[0].sem)){
		x->vags[0].sem=true;
		int pj=x->vags[0].pasajeros;
		if (pj>0){
			x->vags[0].pasajeros=pj-1;
		}
		x->vags[0].sem=false;
	}
	while ((x->vags[1].sem)){

	}
	if (!(x->vags[1].sem)){
		x->vags[1].sem=true;
		int pj=x->vags[1].pasajeros;
		if (pj>0){
			x->vags[1].pasajeros=pj-1;
		}
		x->vags[1].sem=false;
	}
}
void VagonBus(Bus z,int es,int vag ){
	z.ruta[es]->vags[vag];

}
