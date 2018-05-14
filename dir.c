#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

void listarDir (char *caminho){
	DIR *diretorio;
	diretorio = opendir(caminho);
	struct dirent *d;
	if (diretorio == NULL)
		return;
	while ((d = readdir(diretorio)) != NULL){
		if (strncmp(d->d_name, ".", 1) == 0){
			continue;
		}
		printf("%s\n", d->d_name);
	}
}

bool criarDiretorio (char *nome, char *status){
	if (mkdir(nome) == 0){
		return true;
	}else if(errno == EACCES){	//13
		strcpy(status, "Acesso ao arquivo nao permitido\n");
	}else if(errno == EEXIST){	//17
		strcpy(status, "O arquivo ja existe\n");
	}else if (errno == EFAULT){	//14
		strcpy(status, "Area de endereço nao acessivel\n");
	}else if (errno == EMLINK){	//31
		strcpy(status, "Numero de links para arquivo excedido\n");
	}else if (errno == ENAMETOOLONG){	//38
		strcpy(status, "Nome do arquivo longo demais\n");
	}else if (errno == ENOENT){	//2
		strcpy(status, "O nome do arquivo nao existe\n");
	}else if (errno == ENOMEM){	//12
		strcpy(status, "Memoria insuficiente\n");
	}else if (errno == ENOSPC){	//28
		strcpy(status, "Sem entrada no diretorio\n");
	}else if (errno == ENOTDIR){	//20
		strcpy(status, "Um componente do nome do arquivo nao e um diretorio\n");
	}else if (errno == EPERM){	//1
		strcpy(status, "Sem permissão\n");
	}else if (errno == EROFS){	//30
		strcpy(status, "Permissao invalida\n");
	}
	else{
		strcpy(status, "Desconhecido\n");
	}
	return false;
}

bool mudarDiretorio(char *pasta, char *caminho, char *status){
	if (strcmp(pasta, "..") == 0){
		return true;
	}
	strcat(caminho, "\\");
	strcat(caminho, pasta);
	if(chdir (caminho) == 0){
		return true;
	}else if (errno == ENOENT){
		strcpy(status, "Caminho nao existe\n");
	}else if(errno == ENOMEM){
		strcpy(status, "Nao e um diretorio\n");
	}else {
		strcpy(status, "Desconhecido");
	}
	return false;
}



void main(){
	static char buf[100];
	char *caminho;
	char status[60];
	char comando[5];
	char label[20];
	while (true){
		caminho = getcwd(buf, sizeof(buf));
		printf("<%s>", caminho);
		//gets(comando);
		scanf("%s %s", &comando, &label);
		if (strncmp(comando, "ls", 2) == 0){
			listarDir(caminho);
		}else if (strncmp(comando, "mkdir", 5) == 0){
			if (!(criarDiretorio(label, status))){
				printf("Nao foi possivel criar o diretorio, motivo: \n\t%s\n", status);
			}
		}else if (strcmp(comando, "exit") == 0){
			printf("saindo...");
			break;
		}else if(strcmp (comando, "cd") == 0){
			if(!(mudarDiretorio(label, caminho, status))){
				printf("Nao foi possivel ir para o diretorio, motivo: \n\t%s\n", status);
				printf("errno: %d\n", errno);
				printf("errno diretorio: %d\n", ENOMEM);
			}
		}
		else{
			printf("Opcao invalida\n");
		}
	}
}



