/**
 * 
 * 
 * Colocar structs, defines comuns a clientes e servidor aqui
 * 
 * 
 * */

#define PORT 4000

#define TRUE 1

#define FALSE 0

#define WAITING 2

#define PAYLOAD_SIZE 512

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

#define CLIENT_NAME_SIZE 64

#define FILENAME_SIZE 256

#define TYPE_UPLOAD 10

#define TYPE_DOWNLOAD 20

#define TYPE_DATA 01

#define TYPE_DELETE 30

#define TYPE_EXIT 90

#define TYPE_LIST_SERVER 40

#define TYPE_LIST_CLIENT 50

#define  TYPE_GET_SYNC_DIR 60


typedef struct packet {
    uint16_t type; // Tipo do pacote ( DATA | CMD )
    uint16_t seqn; // Numero de sequencia
    uint16_t length; // Comprimento do payload
    uint32_t total_size; // Numero total de fragmentos
    char clientName[CLIENT_NAME_SIZE];
    char fileName[FILENAME_SIZE];
    char _payload[PAYLOAD_SIZE]; // Dados do pacote
} packet;


#define PACKET_SIZE (sizeof (struct packet))

void serializePacket(packet* inPacket, char* serialized);

void deserializePacket(packet* outPacket, char* serialized);

void upload(int sockfd, char* path, char* clientName, int server);

void uploadCommand(int sockfd, char* path, char* clientName, int server);

void download(int sockfd, char* fileName, char* clientName, int server);

void downloadCommand(int sockfd, char* path, char* clientName, int server);

void deleteCommand(int sockfd,char *path, char *clientName);

void delete(int sockfd, char* fileName, char* pathUser);

void list_serverCommand(int sockfd, char *clientName);

void list_files(int sockfd,char *pathToUser, int server);

void list_clientCommand(int sockfd, char *clientName);

/*
  Lança uma thread para ficar no watcher no path de argumento
*/
void *inotifyWatcher(void *pathToWatch);

/*
  Verifica se o usuario já tem o diretorio criado na pasta User com seu nome, e se não tiver já o cria
*/
int checkAndCreateDir(char *pathName);
/*
 Retorna o nome do arquivo a partir do path
*/
char* getFileName(char *path);
/*
 Atribui valores ao packet 
*/
void setPacket(packet *packetToSet,int type, int seqn, int length, int total_size, char* fileName, char* clientName, char* payload);
/*
 Atribui caminho relativo ate arquivo
*/
void pathToFile(char* pathToFile ,char* pathUser, char* fileName);
