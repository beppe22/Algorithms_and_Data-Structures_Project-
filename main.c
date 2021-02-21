//
//  main.c
//
//  Created by Giuseppe Italia on 23/08/2019.
//  Copyright © 2019 Giuseppe Italia. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*typedef struct{
    char ent[10];
} Entita;*/





typedef enum{false,true} tipobool;



//ALBERO DELLE ENTITA
typedef struct nodoEnt* arbolEnt;
struct nodoEnt
{
    char raiz[50];
    arbolEnt hi, hd;
};



//ALBERI DELLE LISTA

typedef struct nodo* arbol;
typedef struct{
    int nVolte_ricevente;
    char ent_dest[50];
    arbolEnt albero_ent_orig;
} raiz_albero_lista;

struct nodo
{
    raiz_albero_lista raiz;
    arbol hi, hd;
};


//STRUTTURA DELLE LISTE CHE CONTIENE GLI "ALBERI DELLA LISTA "

typedef struct elem_lista* lista;
struct elem_lista{
    char id_relazione[50];
    int mas;
    arbol albero_ent_dest;
    lista next;
};

//LISTA DELLE ENTITA CHE PIU RICEVENO LE RELAZIONI

typedef struct elem_lista_ent* lista_ent;
struct elem_lista_ent{
    char ent[50];
    lista_ent next;
};


//DICHIARAZIONE FUNZIONI
int ifVuotoArbolEnt(arbolEnt a);
void addent(arbolEnt *a, char* x);
void eliminar_entDest(arbol* a, arbol* aux);
tipobool ifRelazionePresente(lista *a, char* rel);
void inserisci_in_testa(lista *b, char* k, char* ent_orig, char* ent_dest);
void inserisci(lista *b, char* rel, char* ent_orig, char* ent_dest);
int ifVuotoArbol(arbol a);
arbolEnt ifEntitaPresente(arbolEnt *a, char* ent);
void add_ent(arbol *a, char* ent_dest, char* ent_orig);
arbol ifEntitaDestPresente(arbol *a, char* ent);
void report(lista *l);
int get_entDest(arbol *a, int mas, lista_ent *l_ent);
void inserisci_in_testa_ent(lista_ent *l_ent, char* ent);
void inserisci_ent(lista_ent *l_ent, char* ent);
void delrel(lista *l, char* ent_orig, char* ent_dest, char* rel);
void cancellare_entDest(arbol* a, char* ent_dest);
void eliminar_ent(arbolEnt* a, arbolEnt* aux);
void cancellare_ent(arbolEnt* a, char * ent);
void elimina_tipoRel(lista *l, char *rel);
void delent(lista *l, arbolEnt *a, char *ent);
void pulisci_dallEntita(lista *l,lista *p,arbol *a, char *ent);
tipobool if_altroMasPresente(arbol *a, int mas,  char* ent);
int controllo_mas(arbol *a, int mas);
int massimo(int a, int b);

//COMANDI
void addent(arbolEnt *a, char* x) //c'e un errore di base. quando utilizzo questa funzione per l'aggiunta di un entità orig nella struttre delle liste allora non fa niente se questa già esiste!!SBAGLIATO perchè se questa esistesse allora è come se già esistesse la relazione che io sto costruendo, dunque dovrebbe non inserirla(per come questa funz fa) ma infine eliminare il ++ della entità ric nell'albero delle liste
{
    if (ifVuotoArbolEnt(*a))
    {
        *a=(struct nodoEnt*)malloc(sizeof(struct nodoEnt));
        
        strcpy((*a)->raiz,x);
        (*a)->hi=NULL;
        (*a)->hd=NULL;
    }
    else
    {
        if(strcmp((*a)->raiz, x)!=0)
        {
            if (strcmp((*a)->raiz, x)<0)
            {
                addent(&((*a)->hd),x);
            }
            else
            {
                addent(&((*a)->hi),x);
            }
        }
        
    }
    
}



void addrel(arbolEnt *a, lista *l, char* ent_orig, char* ent_dest, char* rel)
{
    
    tipobool trovato;
    lista punt; //COSA dovrei fare con questO? dovrei eliminarlo a fine funzione o lasciandolo così la struttura non vado a creare ogni volta un nuovo puntatore? dico questo per una questione di memoria
    
    trovato=false;
    if(ifEntitaPresente(a, ent_orig)!=NULL && ifEntitaPresente(a, ent_dest)!=NULL)
    {
        
        punt=*l;
        while(punt!=NULL && !trovato)
        {
            //printf("CIAO");
            if(strcmp(punt->id_relazione, rel)==0)
            {
                trovato=true;
                if(!(ifEntitaDestPresente(&punt->albero_ent_dest, ent_dest)!=NULL && ifEntitaPresente(&(ifEntitaDestPresente(&punt->albero_ent_dest, ent_dest)->raiz.albero_ent_orig), ent_orig)!=NULL))
                {
                    
                //NON CONTROLLO SE L'ALBERO è VUOTO PERCHE SE SONO DENTRO QUESTO IF(DUNQUE ESISTE LA RELAZIONE) ALLORA L'ALBERO AVRà ALMENO UNA ENTITA PER DEFINIZIONE DI TRACCIA
                if(ifEntitaDestPresente(&punt->albero_ent_dest, ent_dest)!=NULL)
                {if(ifEntitaDestPresente(&punt->albero_ent_dest, ent_dest)->raiz.nVolte_ricevente+1>punt->mas)
                   punt->mas= ifEntitaDestPresente(&punt->albero_ent_dest, ent_dest)->raiz.nVolte_ricevente+1;
                
                }
                add_ent(&(punt->albero_ent_dest), ent_dest, ent_orig);
                }
                
            }
            punt=punt->next;
        }
        if(!trovato) inserisci(&(*l), rel, ent_orig, ent_dest);
        
    }
        
        
    
    
}

//IN QUESTA FUNZIONE NON STO CONTROLLANDO SE LE ENTITA SONO MONITORATE, NON ME LO CHIEDE
void delrel(lista *l, char* ent_orig, char* ent_dest, char* rel)
{
    //DEVO CAPIRE SE LA RICERCA DI UNA RELAZIONE MADRE(DUNQUE TRAMITE IL NOME) VIENE FATTA ALTRE VOLTA COSI CASOMAIU DA FARE UNA FUNZIONE A PARTE
    lista punt;
    
    punt=*l;
    //ANDREBBE MESSO UN CONTROLLO PER VEDERE SE LE ENTITà SONO MONITORATE COSI DA NON FARGLI FARE TUTTI I CONTROLLI DOPO INUTILMENTE
    while(punt!=NULL)
    {
        if(strcmp(punt->id_relazione, rel)==0)
        {
            
            if((ifEntitaDestPresente(&punt->albero_ent_dest, ent_dest)!=NULL && ifEntitaPresente(&(ifEntitaDestPresente(&punt->albero_ent_dest, ent_dest)->raiz.albero_ent_orig), ent_orig)!=NULL))
            {
                arbol dest=ifEntitaDestPresente(&punt->albero_ent_dest, ent_dest);
                if(dest->raiz.albero_ent_orig->hd==NULL && dest->raiz.albero_ent_orig->hi==NULL)
                {
                if(punt->albero_ent_dest->hd==NULL && punt->albero_ent_dest->hi==NULL)
                {
                    elimina_tipoRel(&(*l), rel);
                }
                    //fai lo stesso controllando n_volte_ricevente
                    else
                    {
                        if(dest->raiz.nVolte_ricevente==punt->mas)
                        {
                            cancellare_entDest(&punt->albero_ent_dest, ent_dest);
                            punt->mas=controllo_mas(&punt->albero_ent_dest, 0);
                        }
                        else cancellare_entDest(&punt->albero_ent_dest, ent_dest);}
                }
                else
                {
                    cancellare_ent(&dest->raiz.albero_ent_orig, ent_orig);
                    if(dest->raiz.nVolte_ricevente==punt->mas)
                    {
                    if(!if_altroMasPresente(&punt->albero_ent_dest, punt->mas, ent_dest))
                        punt->mas--;
                    }
                    dest->raiz.nVolte_ricevente--;
                }
            }
            break;
        }
        punt=punt->next;
    }
    
        
}

void delent(lista *l, arbolEnt *a, char *ent)
{
    if(ifEntitaPresente(&(*a), ent)!=NULL)
    {
        cancellare_ent(&(*a), ent);
        lista nuovo_punt;
        lista punt;
        punt=*l;
        while(punt!=NULL)
        {
            if(ifEntitaDestPresente(&punt->albero_ent_dest, ent)!=NULL)
            {
                arbol dest=ifEntitaDestPresente(&punt->albero_ent_dest, ent);
                if(punt->albero_ent_dest->hd==NULL && punt->albero_ent_dest->hi==NULL)
                {
                    char rel[50]= "";
                    strcpy(rel,punt->id_relazione); //ci puo essere un problema di non eliminazione cartteri quando viene utilizzata più spesso per diverse rel
                    punt=punt->next;
                    elimina_tipoRel(l, rel);
                    continue;
                }
            else {
                if(punt->mas==dest->raiz.nVolte_ricevente)
                {
                    cancellare_entDest(&punt->albero_ent_dest, ent);
                    punt->mas=controllo_mas(&punt->albero_ent_dest, 0);
                }
                else cancellare_entDest(&punt->albero_ent_dest, ent);
                
            }
            }
            nuovo_punt=punt;
            punt=punt->next;
            pulisci_dallEntita(l,&nuovo_punt,&nuovo_punt->albero_ent_dest, ent);
            
        }
    }
}

void pulisci_dallEntita(lista *l,lista *p,arbol *a, char *ent)
{
    tipobool if_eliminare_rel=false, ritorno=false;
    
    
    if(!ifVuotoArbol(*a))
    {
        if_eliminare_rel=false;
        ritorno=false;
    if(ifEntitaPresente(&(*a)->raiz.albero_ent_orig, ent)!=NULL)
    {
        if((*a)->raiz.albero_ent_orig->hd==NULL && (*a)->raiz.albero_ent_orig->hi==NULL)
        {
            if((*p)->albero_ent_dest->hd==NULL && (*p)->albero_ent_dest->hi==NULL)
            {
                if_eliminare_rel=true;
            }
            else {
                if((*p)->mas==(*a)->raiz.nVolte_ricevente)
                {
                    cancellare_entDest((&(*p)->albero_ent_dest), (*a)->raiz.ent_dest);
                    (*p)->mas= controllo_mas(&(*p)->albero_ent_dest, 0);
                }
                else cancellare_entDest(&(*p)->albero_ent_dest, (*a)->raiz.ent_dest);
                if((*a)!=NULL){
                if(ifEntitaPresente(&(*a)->raiz.albero_ent_orig, ent)!=NULL)
                {
                    pulisci_dallEntita(l,p,&(*a), ent);
                    ritorno=true;
                }}
            }
        }
        else
        {
            if((*p)->mas==(*a)->raiz.nVolte_ricevente && !if_altroMasPresente(&(*p)->albero_ent_dest, (*p)->mas, (*a)->raiz.ent_dest))
            {
                (*p)->mas--;
            }
            cancellare_ent(&(*a)->raiz.albero_ent_orig, ent);
            (*a)->raiz.nVolte_ricevente--;
        }
    }
        if(!ritorno){
    if(*a!=NULL)
    {
        pulisci_dallEntita(l,p,&(*a)->hd, ent);
        pulisci_dallEntita(l,p,&(*a)->hi, ent);
        if(if_eliminare_rel) elimina_tipoRel(l, (*p)->id_relazione );
    }
        
        }}
}

void report(lista *l)
{
    lista_ent l_ent=NULL;
    lista punt1=NULL;
    lista_ent punt2=NULL;
    int k=0;
    
    k=0;
    if(*l==NULL) printf("none");
    else{
    punt1=*l;
    while(punt1!=NULL)
    {
        k++;
        l_ent=NULL;
        get_entDest(&punt1->albero_ent_dest, punt1->mas, &l_ent);
        punt2=l_ent;
        if(k==1) printf("%s ", punt1->id_relazione);
        else printf(" %s ", punt1->id_relazione);
        while(punt2!=NULL)
        {
            printf("%s ",punt2->ent);
            punt2=punt2->next;
        }
        printf("%d;", punt1->mas);
        free(l_ent);
        punt1=punt1->next;
        
    }
    }

    printf("\n");
}


//FUNZIONI PERIFERICHE AI COMANDI
    
int get_entDest(arbol *a, int mas, lista_ent *l_ent)
{
    if (ifVuotoArbol(*a))
    {
        return 0;
    }
    //if((*a)->raiz==NULL) return 0;
    if((*a)->raiz.nVolte_ricevente==mas)
        inserisci_ent(l_ent,(*a)->raiz.ent_dest );
    
    return get_entDest(&((*a)->hd),mas,&(*l_ent)) + get_entDest(&((*a)->hi),mas, &(*l_ent));
}



void cancellare_entDest(arbol* a, char * ent_dest)
{
    arbol aux;
    
    if (! ifVuotoArbol(*a))
    {
    if (strcmp(ent_dest ,(*a)->raiz.ent_dest)<0)
        cancellare_entDest(&((*a)->hi), ent_dest);
    else
        {
            if (strcmp(ent_dest ,(*a)->raiz.ent_dest)>0)
            {
                cancellare_entDest(&((*a)->hd), ent_dest);
            }
            else
            {
                aux=*a;
                if (ifVuotoArbol((*a)->hd)) *a=(*a)->hi;
                else
                {
                    if (ifVuotoArbol((*a)->hi)) *a=(*a)->hd;
                    
                    else eliminar_entDest(&((*a)->hi), &aux);
                }
                free(aux);
            }
        }}
}

void cancellare_ent(arbolEnt* a, char * ent)
{
    arbolEnt aux;
    
    if (! ifVuotoArbolEnt(*a))
    {
        if (strcmp(ent,(*a)->raiz)<0)
            cancellare_ent(&((*a)->hi), ent);
        else
        {
            if (strcmp(ent,(*a)->raiz)>0)
            {
                cancellare_ent(&((*a)->hd), ent);
            }
            else
            {
                aux=*a;
                if (ifVuotoArbolEnt((*a)->hd)) *a=(*a)->hi;
                else
                {
                    if (ifVuotoArbolEnt((*a)->hi)) *a=(*a)->hd;
                    
                    else eliminar_ent(&((*a)->hi), &aux);
                }
                free(aux);
            }
        }}
}


void add_ent(arbol *a, char* ent_dest, char* ent_orig) //funzione per aggiungere le entità negli alberi delle liste
{
    if (ifVuotoArbol(*a))
    {
        //printf("CIAO");
        *a=(struct nodo*)malloc(sizeof(struct nodo));
        (*a)->raiz.albero_ent_orig=NULL;
        
        strcpy((*a)->raiz.ent_dest,ent_dest);
        (*a)->raiz.nVolte_ricevente=1;
        addent(&(*a)->raiz.albero_ent_orig , ent_orig);
        (*a)->hi=NULL;
        (*a)->hd=NULL;
    }
    else
    {
        if(strcmp((*a)->raiz.ent_dest , ent_dest)!=0)
        {
            if (strcmp((*a)->raiz.ent_dest , ent_dest)<0)
            {
                add_ent(&((*a)->hd),ent_dest, ent_orig);
            }
            else
            {
                add_ent(&((*a)->hi),ent_dest, ent_orig);
            }
        }
        else
        {
            if(!ifEntitaPresente(&(*a)->raiz.albero_ent_orig, ent_orig))
            {
            (*a)->raiz.nVolte_ricevente++;
            addent(&(*a)->raiz.albero_ent_orig , ent_orig);
            }
        }
    }
        
}



//FUNZIONI PER ALBERI
void eliminar_ent(arbolEnt* a, arbolEnt* aux)
{
    if (! ifVuotoArbolEnt((*a)->hd))
        eliminar_ent(&((*a)->hd), aux);
    else
    {
        strcpy((*aux)->raiz,(*a)->raiz);
        *aux=*a;
        *a=(*a)->hi;
    }
}

void eliminar_entDest(arbol* a, arbol* aux)
{
    if (! ifVuotoArbol((*a)->hd))
        eliminar_entDest(&((*a)->hd), aux);
    else
    {
        (*aux)->raiz.albero_ent_orig=(*a)->raiz.albero_ent_orig;
        (*aux)->raiz.nVolte_ricevente=(*a)->raiz.nVolte_ricevente;
        strcpy((*aux)->raiz.ent_dest,(*a)->raiz.ent_dest);
        *aux=*a;
        *a=(*a)->hi;
    }
}

int ifVuotoArbolEnt(arbolEnt a)
{
    return (a==NULL);
}

int ifVuotoArbol(arbol a)
{
    return (a==NULL);
}

tipobool if_altroMasPresente(arbol *a, int mas,  char* ent) //NON SI SA SE FUNZIONA
{
    if (ifVuotoArbol(*a))
    {
        return false;
    }
    
    if(strcmp((*a)->raiz.ent_dest , ent)!=0 && mas==(*a)->raiz.nVolte_ricevente) return true;
    return if_altroMasPresente(&((*a)->hd),mas, ent) + if_altroMasPresente(&((*a)->hi), mas,ent);
    
}

int controllo_mas(arbol *a, int mas)
{
    if(ifVuotoArbol(*a)) return mas;
    if((*a)->raiz.nVolte_ricevente>mas)
    {
        mas=(*a)->raiz.nVolte_ricevente;
    }
    return massimo(controllo_mas(&((*a)->hd),mas),controllo_mas(&((*a)->hi),mas));
    
}

int massimo(int a, int b)
{
    if(a>b) return a;
    else return b;
}


arbolEnt ifEntitaPresente(arbolEnt *a, char* ent)
{
    if (ifVuotoArbolEnt(*a))
    {
        return NULL;
    }
    
    if(strcmp((*a)->raiz , ent)!=0)
        {
            if (strcmp((*a)->raiz, ent)<0)
            {
                return ifEntitaPresente(&((*a)->hd),ent);
            }
            else
            {
                return ifEntitaPresente(&((*a)->hi),ent);
            }
    }
    else return *a;
}
                                 
arbol ifEntitaDestPresente(arbol *a, char* ent)
{
    if (ifVuotoArbol(*a))
    {
        return NULL;
    }
    
    if(strcmp((*a)->raiz.ent_dest , ent)!=0)
    {
        if (strcmp((*a)->raiz.ent_dest, ent)<0)
        {
            return ifEntitaDestPresente(&((*a)->hd),ent);
        }
        else
        {
            return ifEntitaDestPresente(&((*a)->hi),ent);
        }
    }
    else return *a;
}






//FUNZIONI LISTE


void elimina_tipoRel(lista *l, char *rel)
{
    lista punt, dietro;
    
    punt=*l;
    if(strcmp(punt->id_relazione, rel)==0)
    {
        *l=punt->next;
        free(punt);
        
    }
    else{
        punt=punt->next;
        dietro=*l;
        while(strcmp(punt->id_relazione, rel)!=0)
        {
            punt=punt->next;
            dietro=dietro->next;
        }
        dietro->next=punt->next;
        free(punt);
    }
}


void inserisci_in_testa(lista *l, char* rel, char*  ent_orig, char* ent_dest)
{
    lista punt=NULL;
    
    //punt=NULL;
    punt=(struct elem_lista*)malloc(sizeof(struct elem_lista));
    punt->albero_ent_dest=NULL;
    punt->mas=1;
    
    strcpy( punt->id_relazione,rel);
    add_ent(&(punt->albero_ent_dest), ent_dest, ent_orig);
    punt->next=*l;
    *l=punt;
}



void inserisci(lista *l, char* rel, char* ent_orig, char* ent_dest)
{
    lista punt=NULL, new=NULL;
    
    if(*l==NULL)
    {
        *l=(struct elem_lista*)malloc(sizeof(struct elem_lista));
        (*l)->albero_ent_dest=NULL;
        (*l)->mas=1;
        strcpy((*l)->id_relazione,rel);
        add_ent(&((*l)->albero_ent_dest), ent_dest, ent_orig);
        (*l)->next=NULL;
    }
    else{//SE NON E' NULLA
    punt=*l;
    
    if(strcmp(rel, punt->id_relazione)<0)
    {
        //punt=NULL;
        inserisci_in_testa(l,rel, ent_orig, ent_dest);
        
        
    }
    else{
    while(punt->next!=NULL)
    {
        
        if(strcmp(rel, punt->next->id_relazione)>0) punt=punt->next;
        else
        {
            new=(struct elem_lista*)malloc(sizeof(struct elem_lista));
            new->albero_ent_dest=NULL;
            new->mas=1;
            
            strcpy(new->id_relazione,rel);
            add_ent(&(new->albero_ent_dest), ent_dest, ent_orig);
            new->next=punt->next;
            punt->next=new;
            break;
        }
        
    }
    //INSERISCO IN CODA ALLA LISTA
        
    if(punt->next==NULL)
    {
    new=(struct elem_lista*)malloc(sizeof(struct elem_lista));
    new->albero_ent_dest=NULL;
    new->mas=1;
   
    strcpy(new->id_relazione,rel);
    add_ent(&(new->albero_ent_dest), ent_dest, ent_orig);
    new->next=NULL;
    punt->next=new;
    
    }}}
}


//FUNZIONI LISTE PER LISTA ENTITA'
void inserisci_in_testa_ent(lista_ent *l_ent, char* ent)
{
    lista_ent punt=NULL;
    
    
    punt=(struct elem_lista_ent*)malloc(sizeof(struct elem_lista_ent));
    
    strcpy( punt->ent,ent);
    punt->next=*l_ent;
    *l_ent=punt;
}



void inserisci_ent(lista_ent *l_ent, char* ent)
{
    lista_ent punt=NULL, new=NULL;
    
    if(*l_ent==NULL)
    {
        *l_ent=(struct elem_lista_ent*)malloc(sizeof(struct elem_lista_ent));
        
        strcpy((*l_ent)->ent,ent);
        (*l_ent)->next=NULL;
    }
    else{//SE NON E' NULLA
        punt=*l_ent;
        
        if(strcmp(ent, punt->ent)<0)
        {
            //punt=NULL;
            inserisci_in_testa_ent(l_ent,ent);
            
            
        }
        else{
            while(punt->next!=NULL)
            {
                
                if(strcmp(ent, punt->next->ent)>0) punt=punt->next;
                else
                {
                    new=(struct elem_lista_ent*)malloc(sizeof(struct elem_lista_ent));
                    
                    strcpy(new->ent,ent);
                    new->next=punt->next;
                    punt->next=new;
                    
                    break;
                }
                
            }
            //INSERISCO IN CODA ALLA LISTA
            if(punt->next==NULL)
            {
                new=(struct elem_lista_ent*)malloc(sizeof(struct elem_lista_ent));
                
                strcpy(new->ent,ent);
                new->next=NULL;
                punt->next=new;
            }}}
}

//__________________________________________________
    
int main(int argc, const char * argv[]) {
    // insert code here...
    
    
    char ent_orig[50]="";
    char ent_dest[50]="";
    char id_rel[50]="";
    char comando[100]="";
    char ent[50]="";
    int k=0,i=0;
    arbolEnt arbol=NULL;
    lista Lista=NULL;
    
    do
    {
        fgets(comando, 100, stdin);
        if(strstr(comando, "addent")!=NULL)
        {
            
            ent[0]='"';
            k=1;
            for(i=8;comando[i]!='"';i++)
            {
                ent[k]=comando[i];
                k++;
            }
            ent[k]='"';
            ent[k+1]='\0';
            addent(&arbol, ent);
        }
        else if(strstr(comando, "delent")!=NULL)
        {
            ent[0]='"';
            k=1;
            for(i=8;comando[i]!='"';i++)
            {
                ent[k]=comando[i];
                k++;
            }
            ent[k]='"';
            ent[k+1]='\0';
            delent(&Lista, &arbol, ent);
        }
        else if(strstr(comando, "addrel")!=NULL)
        {
            
            k=7;
            i=0;
            
            
            do
            {
                ent_orig[i]=comando[k];
                i++;
                k++;
            }while(comando[k]!='"');
            ent_orig[i]='"';
            ent_orig[i+1]='\0';
            k=k+2;
            i=0;
            do
            {
                ent_dest[i]=comando[k];
                i++;
                k++;
            }while(comando[k]!='"');
            ent_dest[i]='"';
            ent_dest[i+1]='\0';
            k=k+2;
            i=0;
            do
            {
                id_rel[i]=comando[k];
                i++;
                k++;
            }while(comando[k]!='"');
            id_rel[i]='"';
            id_rel[i+1]='\0';
           addrel(&arbol, &Lista, ent_orig, ent_dest, id_rel);
         
        }
        else if(strstr(comando,"report")!=NULL) report(&Lista);
        else if(strstr(comando, "delrel")!=NULL)
        {
            k=7;
            i=0;
            do
            {
                ent_orig[i]=comando[k];
                i++;
                k++;
            }while(comando[k]!='"');
            ent_orig[i]='"';
            ent_orig[i+1]='\0';
            k=k+2;
            i=0;
            do
            {
                ent_dest[i]=comando[k];
                i++;
                k++;
            }while(comando[k]!='"');
            ent_dest[i]='"';
            ent_dest[i+1]='\0';
            k=k+2;
            i=0;
            do
            {
                id_rel[i]=comando[k];
                i++;
                k++;
            }while(comando[k]!='"');
            id_rel[i]='"';
            id_rel[i+1]='\0';
            delrel(&Lista, ent_orig, ent_dest, id_rel);
        }
        
        
    }while(strstr(comando, "end")==NULL);
    

    
    
   
    
    return 0;
}
