/*
  Este projeto é baseado no
  exemplo Basic ESP8266 MQTT
  da biblica PubSubClient

  Projeto: MQTT_NODEMCU_MQTTBOX
  

*/

#include <ESP8266WiFi.h>        //Biblioteca para o NODEMCU se conectar ao WiFi
#include <PubSubClient.h>       //Biblioteca de publicador/subscrito


//-------------Defines------------------------------------
#define MSG_BUFFER_SIZE  (50)   //Define para o compilador o tamanho da buffer de recebimento
#define BUILTIN_LED D0          //pino onde o led está ligado no NodeMCU


//---------Configuração para WIFI-------------------------
const char* ssid = "GABRIEL";          //nome da rede wifi
const char* password = "12345678";      //senha da rede wifi
//--------fim Configuração para WIFI----------------------


//----------Endereço do Broker----------------------------
const char* mqtt_server = "broker.mqtt-dashboard.com";  //endereço do servidor mqtt
//------------fim endereço do Broker---------------------


//----------Objetos---------------------------------------
WiFiClient espClient;             //Cliente ESP Wifi
PubSubClient client(espClient);   //Cliente ESP publicador/sbscrito
//---------fim Objetos------------------------------------



//---------Variavéis Globais-----------------------------
unsigned long lastMsg = 0;      //Variável que armazenara o valor do tempo da ultima mensagem enviada
char msg[MSG_BUFFER_SIZE];      //A variável será do tipo char e o tamanho definido pelo buffer de recebimento

const char rele = 5;            //Definição do pino onde será conectado o rele no NODEMCU D1
float ldr;                      //Variável que armazenará o valor lido do ldr. Necessário ser em float, pois a biblioteca define que seja esse tipo de dado enviado

boolean   flagRele,             //Variáveis do tipo booleanas para sinalizar quando os eventos relativos a ação do rele e led acontecem
          flagBUILTIN_LED;
//---------fim Variáveis Globais-------------------------



//---------Prótotipos das Funções-----------------------
void setup_wifi();                    //Função de configuração de conexão do wifi
void callback(char* topic,            //Função de Callback
              byte* payload,
              unsigned int length);
void reconnect();                     //Função de Reconexão com o Broker
void cond_rele();                     //Função de chamada para a condição do status do rele
void cond_BUILTIN_LED();              //Função de chamada para a condição do status do led onboard no NODEMCU
//----fim prototipos---------





void setup() {

  pinMode(BUILTIN_LED, OUTPUT);             //Configura o pino do led onboard do NODEMCU com saida
  pinMode(rele, OUTPUT);                    //Configura o pino onde o rele está ligado como saída
  Serial.begin(115200);                     //Velocidade de comunicação serial
  setup_wifi();                             //Chamada da função de configuração e conexão do wifi
  client.setServer(mqtt_server, 1883);      //Seta o servidor mqtt de port 1883
  client.setCallback(callback);             //Seta mensagem de retorno
}


void loop() {

  if (!client.connected()) {                //Condição de verificação de conexão. Se client for diferente conectado
    reconnect();                            //então chama a função de reconexão
  }
  client.loop();                            //Permitir o processamento e publicação de mensagens e dados


  unsigned long now = millis();             //millis() é uma função nativa da IDE do arduino que conta o tempo em que o programa está rodando. "now" armazena esse valor
  if (now - lastMsg > 4000) {               //Condição de verificação de tempo decorrido. Se now - lastMsg for maior que 2000
    lastMsg = now;                          //lastMsg armazena o valor de now
    cond_rele();                            //a função de cond_rele é chamada
    cond_BUILTIN_LED();                     //a função de cond_BUILTIN_LED é chamada
    sprintf (msg, "%.0f", ldr);             //salva o valor da leitura do LDR na mensagem
    Serial.print("Publish message: ");      //Printa na serial a mensagem entre aspas
    Serial.println(msg);                    //Printa na serial a informação contida em msg
    client.publish("casa/ldr", msg);        //Publica a mensagem no topico casa/ldr
  }
}


//------------Funções---------------------


void setup_wifi() {

  delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);               //printa o nome dontido na variável ssid

  WiFi.mode(WIFI_STA);                        //Configura o modo do NODEMCU como station
  WiFi.begin(ssid, password);       //Inicia a conexão do wifi com as informações passada de ssid e password

  while (WiFi.status() != WL_CONNECTED) {     //Laço de repetição que verifica o status do Wifi. Se o status for diferente de WL_CONNECTED
    delay(500);                               //Espera 0,5 segundos
    Serial.print(".");                        //Printa na serial os "." para mostrar que está processando a conexão
  }


  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());             //Printa na serial o ip do NODEMCU na rede local

}//----fim wifi_setup-------------


void callback(char* topic, byte* payload, unsigned int length) {      //Inicaliza a função de retorno de mensagem. Está função recebe como parametros,
  //topic, payoad, e length
  Serial.print("Message arrived [");                                  //Printa o nome do tópico recebido
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {                                  //Laço de repetição condicionado ao tamanho da mensagem
    Serial.print((char)payload[i]);                                   //Printa na serial cada caracter da informação util da mensagem
  }
  Serial.println();

  if ((String)topic == "casa/BUILTIN_LED") {                          //se o topico for o casa/BUILTIN_LED
    if ((char)payload[0] == '1') {                                    //Se o payload for 1
      flagBUILTIN_LED = true;                                         //seta a flag flagBUILTIN_LED como true
      snprintf (msg, MSG_BUFFER_SIZE, "BUILTIN_LED ligado");          //Printa na serial e salva a mensagem em msg indicando que o led onboard está ligado
      Serial.print("Publica mensagem: ");
      Serial.println(msg);
      client.publish("casa/BUILTIN_LED", msg);                        //publica no topico casa/BUILTIN_LED a mensagem
    }
    if ((char)payload[0] == '0') {                                    //se o payload for 0
      flagBUILTIN_LED = false;                                        //seta a flag flagBUILTIN_LED como false
      snprintf (msg, MSG_BUFFER_SIZE, "BUILTIN_LED desligado");       //Printa na serial e salva a mensagem em msg indicando que o led onboard está desligado
      Serial.print("Publica mensagem: ");
      Serial.println(msg);
      client.publish("casa/BUILTIN_LED", msg);                        //publica no topico casa/BUILTIN_LED a mensagem
    }
  }

  if ((String)topic == "casa/rele") {

    if ((char)payload[0] == '1') {                                    //se o payload for 3
      flagRele = true;                                                //seta a flag flagRele como true
      snprintf (msg, MSG_BUFFER_SIZE, "Rele Habilitado");             //printa na serial e salva a mensagem em msg indicando que o rele está habilitado
      Serial.print("Publica mensagem: ");
      Serial.println(msg);
      client.publish("casa/rele", msg);                               //publica no topico casa/rele a mensagem
    }

    if ((char)payload[0] == '0') {                                     //se o payload for 2
      flagRele = false;                                                // //seta a flag flagRele como false
      snprintf (msg, MSG_BUFFER_SIZE, "Rele Desabillitado");           //printa na serial e salva a mensagem em msg indicando que o rele está desabilitado
      Serial.print("Publica mensagem: ");
      Serial.println(msg);
      client.publish("casa/rele", msg);                               ////publica no topico casa/rele a mensagem

    }
  }

}//----------fim callback-----------------



void reconnect() {

  while (!client.connected()) {                             //Laço de repetição que verifica se client é diferente de conectado
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";                    //Todo cliente tem um id. Aqui é criado esse id de forma randomica
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {                //Se for realizada a conexão
      Serial.println("connected");
      client.subscribe("casa/rele");                       //Realiza a subscrição no topico casa/rele
      client.subscribe("casa/BUILTIN_LED");                //Realiza a subscrição no topico casa/BUILTIN_LED

    } else {
      Serial.print("falha na conexao com o broker, rc=");
      Serial.print(client.state());                       //printa o status do client
      Serial.println(" Nova tentativa em 5 segundos");
      delay(5000);
    }
  }
}//-------------------fim reconnect---------------------------


void cond_rele() {
  char aux_rele_ligado = 0;                     //variavel para printar apenas uma vez a informação do estado do rele ligado
  char aux_rele_desligado = 0;                  //variavel para printar apenas uma vez a informação do estado do rele desligado

  ldr = analogRead(A0);                         //Realiza a leitura analogica do LDR
  delay(100);
  if ((ldr < 280) && (flagRele == true)) {      //Se a leitura do ldr for menor que 200 e a flagRele que é verificada na função de callback for verdadeira
    aux_rele_desligado = 0;                     //coloca o valor zero na variavel aux_rele_desligado
    digitalWrite(rele, HIGH);                   //rele é ligado
    aux_rele_ligado++;                          //incrementa a variavel aux
    if (aux_rele_ligado == 1) {                 //somente se a variavel aux_rele_ligado for igual a 1
      sprintf (msg, "Rele Ligado");             //Salva a mensagem Rele ligado em msg
      Serial.print("Publish message: ");        //Printa na serial a mensagem entre aspas
      Serial.println(msg);                      //Printa na serial a informação contida em msg
      client.publish("casa/statusRele", msg);   //Publica a mensagem no topico casa/statusRele
    }

  } else {                                      //senão
    aux_rele_ligado = 0;                        //coloca em zero a variavel aux_rele_ligado
    digitalWrite(rele, LOW);                    //rele é desligado
    aux_rele_desligado++;                       //incrementa a variavel aux_rele_desligado
    if (aux_rele_desligado == 1) {              //somente se a variavel aux_rele_desligado for igual a 1
      sprintf (msg, "Rele Desligado");          //Salva a mensagem Rele desligado em msg
      Serial.print("Publish message: ");        //Printa na serial a mensagem entre aspas
      Serial.println(msg);                      //Printa na serial a informação contida em msg
      client.publish("casa/statusRele", msg);   //Publica a mensagem no topico casa/statusRele

    }
  }
}//------fim cond_rele--------------



void cond_BUILTIN_LED() {

  if (flagBUILTIN_LED == true) {                //Apenas uma condição para ativar o led: se a flagBUILTIN_LED que é verificada na função de callback for verdadeira
    digitalWrite(BUILTIN_LED, LOW);             //O led onboard da NODEMCU é ativada com nível baixo. Logo, será ligdo o led
  } else {                                      //Senão
    digitalWrite(BUILTIN_LED, HIGH);            //Desliga o led
  }
}//----fim cond_BUILTIN_LED--------
