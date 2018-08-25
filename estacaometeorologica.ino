
#include "DHT.h" // Biblioteca utilizada para comunicação com sensor de temperatura
#define PINODHT 2     // Em qual pino o sensor está conectado
#define PINOUMIDADE A1 // Pino onde conectamos o sensor de umidade
#define DHTTYPE DHT11   // DHT 11
#define TEMPOLEITURA 5000 // Tempo de leitura definido em millissegundos


DHT dht(PINODHT, DHTTYPE); // Instância do sensor  dht


float temperaturaCelsius; // Variável responsável por armazenar a temperatura
int umidade; // Variável responsável por armazenar a umidade 
unsigned long tempoAnterior; // Variável responsável por controlar a leitura do sensor, sem a utilização de delay

bool lerTemperatura(){ // Função responsável pela leitura do sensor
   umidade = dht.readHumidity(); // Leitura da temperatura
   temperaturaCelsius = dht.readTemperature(); // Leitura da umidade
  if (isnan(umidade) || isnan(temperaturaCelsius)) { // Se o valor retornado pela leitura da temperatura e da umidade não for um número, significa que houve um erro ao ler do sensor
    return false; // Retornamos  falso caso a leitura não tenha sido feita com sucesso
  }
  else{
    return true; // Retornamos verdadeiro caso a leitura tenha sido feita com sucesso
  }
}


bool detectarChuva(){ // Função responsável por detectar se está chovendo ou não
  int umidade = analogRead(PINOUMIDADE); // Leitura do pino analógico onde o sensor de chuva está conectado
  if (umidade>500){ // Se a umidade lida for maior que 500 podemos considerar que não foi detectado respingos de água sobre a superficie do sensor, logo não está chovendo
    return false; // Retornamos falso caso o sensor não detecte a possibilidade de chuva
  }
  else{ // Caso contrário significa que já pode estar chovendo
    return true; // Retornamos verdadeiro caso o sensor detecte a possibilidade de chuva
  }
}

void setup() {
  Serial.begin(9600); // Inicialização da comunicação serial 
  dht.begin(); // Inicialização do sensor de temperatura
}

void loop() {
  unsigned long tempoAtual = millis(); // Armazenamos o tempo atual do microcontrolador, através da função millis

  if (tempoAtual - tempoAnterior >= TEMPOLEITURA){  // se a diferença entre o tempo atual, do microcontrolador menos o ultimo tempo lido, for maior ou igual ao tempo de leitura, significa que devemos executar o bloco if
    tempoAnterior = tempoAtual; // registramos o último tempo lido antes do bloco ser executado
    bool statusLeitura = lerTemperatura(); // Função criada para ler o sensor de temperatura
    if (statusLeitura==false){ // Se a função retornar false significa que houve um erro na leitura do sensor, logo não iremos mostrar as variáveis lidas
      Serial.println("Falha na leitura do sensor DHT11"); // Print da mensagem de erro ao ler o sensor
    }
    else{ // Caso contrário mostramos as variáveis de temperatura e umidade lidas
      Serial.print("Temperatura: ");
      Serial.println(temperaturaCelsius);
      Serial.print("Umidade: ");
      Serial.println(umidade);
    }
    bool statusChuva = detectarChuva(); // Função detecta chuva informa se foi detectada chuva ou não
    if (statusChuva==true){ // Caso o sensor detecte a possibildade de chuva a mensagem está chovendo será exibida
      Serial.println("Está chovendo");
    }
    else{ // caso contrário a menasgem não está chovendo será exibida
      Serial.println("Não está chovendo");
    }
  }
}
