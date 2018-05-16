#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <conio.h>

void listarDir (char *caminho, int label){
	DIR *diretorio;
	diretorio = opendir(caminho);
	struct dirent *d;
	if (diretorio == NULL)
		return;
	while ((d = readdir(diretorio)) != NULL){
		if (strncmp(d->d_name, ".", 1) == 0){
			continue;
		}
		for (int i = 1; i <= label; i++){
			printf("->");
		}
		printf("%s\n", d->d_name);
		if (d->d_type != 0){
			strcat(caminho, "\\");
			strcat(caminho, d->d_name);
			int nlabel = label+1;
			listarDir(caminho, nlabel);
		}
	}
	closedir(diretorio);
}

bool removerDiretorio(char *nome, char *status){
	if(rmdir(nome) == 0){
		return true;
	}else if(errno == EACCES){	//13
		strcpy(status, "Acesso ao arquivo nao permitido\n");
	}else if(errno == EBUSY){	//17
		strcpy(status, "Acesso em uso por outro processo ou pelo sistema\n");
	}else if (errno == EFAULT){	//14
		strcpy(status, "Area de endereço nao acessivel\n");
	}else if (errno == EINVAL){	//31
		strcpy(status, "Valor nao valido\n");
	}else if (errno == ENAMETOOLONG){	//38
		strcpy(status, "Nome do arquivo longo demais\n");
	}else if (errno == ENOENT){	//2
		strcpy(status, "O nome do arquivo nao existe\n");
	}else if (errno == ENOMEM){	//12
		strcpy(status, "Memoria insuficiente\n");
	}else if (errno == ENOTEMPTY){	//28
		strcpy(status, "Pasta nao esta vazia\n");
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

bool mudarDiretorio(char *pasta, char *status){
	if(chdir(pasta) == 0){
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

void informacoes(){
	printf("Comandos:\n");
	printf("\033[35m\tshow: \033[32mmostrar os arquivos dos diretorios\n");
	printf("\033[35m\tmake NOME_PASTA: \033[32mcriar uma pasta\n");
	printf("\033[35m\tgo [NOME_PASTA\\CAMINHO]: \033[32mir para a pasta ou caminho\n");
	printf("\033[35m\tdel NOME_PASTA: \033[32mdeleta a pasta\n");
	printf("\033[35m\texit: \033[32msai do console\n");
}



void main(){
	char caminho[100];
	char status[60];
	char comando[20];
	char *label;
	label = NULL;
	system("cls");
	//system("color f0");
	while (true){
		printf("\033[32m");
		strcpy(caminho,getcwd(caminho, sizeof(caminho)));
		printf("<%s>", caminho);
		gets(comando);
		//scanf("%s %s", &comando, &label);
		if (strncmp(comando, "show", 4) == 0){
			listarDir(caminho, 0);
		}else if (strncmp(comando, "make", 4) == 0){
			if (strlen(comando) > 5){
				int j = 0;
				label = (char *)malloc(sizeof(char) * (strlen(comando) - 5));
				for (int i = 5; i < strlen(comando); i++){
					label[j] = comando[i];
					j++;
				}
				label[j] = '\0';
			}
			if (!(criarDiretorio(label, status))){
				printf("\aNao foi possivel criar o diretorio, motivo: \n\t%s\n", status);
			}
			free(label);
		}else if(strncmp (comando, "go", 2) == 0){
			if (strlen(comando) > 3){
				int j = 0;
				label = (char *)malloc(sizeof(char) * (strlen(comando) - 5));
				for (int i = 3; i < strlen(comando); i++){
					label[j] = comando[i];
					j++;
				}
				label[j] = '\0';
			}
			if(!(mudarDiretorio(label, status))){
				printf("\aNao foi possivel ir para o diretorio, motivo: \n\t%s\n", status);
			}
			free(label);
		}else if(strncmp(comando, "del", 3) == 0){
			if (strlen(comando) > 3){
				int j = 0;
				label = (char *)malloc(sizeof(char) * (strlen(comando) - 4));
				for (int i = 4; i < strlen(comando); i++){
					label[j] = comando[i];
					j++;
				}
				label[j] = '\0';
			}
			if(!(removerDiretorio(label, status))){
				printf("\aNao foi possivel remover o diretorio, motivo: \n\t%s\n", status);
			}
		}else if (strcmp(comando, "exit") == 0){
			printf("saindo...");
			printf("\033[37m ");
			//system("color 07");
			//system("cls");
			break;
		}else if(strncmp(comando, "help", 4) == 0){
			informacoes();
		}
		else{
			printf("\aOpcao invalida\n");
		}
	}
}



