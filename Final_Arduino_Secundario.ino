const int BotaoIncremento = 7;
const int BotaoTentativa = 6;
int NumeroEscolhido = 0;

int Incremento_State = LOW;
int Incremento_Last_State = LOW;
unsigned long Incremento_LastDebounceTime = 0;
unsigned long Incremento_DebounceDelay = 50;

int Tentativa_State = LOW;
int Tentativa_Last_State = LOW;
unsigned long Tentativa_LastDebounceTime = 0;
unsigned long Tentativa_DebounceDelay = 50;

void setup() {
  Serial.begin(9600);
  for (int led = 8; led <= 12; led++){                              
    pinMode(led, OUTPUT);          
  ligar_leds();                                   // Inicialmente os Leds estão Ligados
  }
  pinMode(BotaoIncremento, INPUT_PULLUP);
  pinMode(BotaoTentativa, INPUT_PULLUP);
}

void loop() {
  if (Serial.available() > 0) {                   // Verifica se há dados disponíveis para leitura na porta-série
    char ler = Serial.read();
    if (ler == 'S') {                             // O Arduíno Secundário recebe o Caracter 'S' e o jogo começa, sendo os LEDs piscados três vezes
      NumeroEscolhido = 0;                        // Reset no Número Escolhido, caso se for reinício do Jogo
      for (int i = 1; i <= 3; i++) {
        desligar_leds();
        delay(300);
        ligar_leds();
        delay(300);
      desligar_leds();
    }
   }
    if (ler == 'Y') {              // O Arduíno Secundário recebe o Caracter 'Y', indicando que o número escolhido está certo, acendendo todos os Leds
      ligar_leds();
    }
    else {                         // O Arduíno Secundário recebe o único Caracter diferente de 'Y', ou seja o caracter 'N', indicando que o número escolhido está errado, apagando os Leds e tentando do início
	    NumeroEscolhido = 0;
      desligar_leds(); 
    }
  }

  // Botão de Incremento
  int LerIncremento = digitalRead(BotaoIncremento);
  if ((millis() - Incremento_LastDebounceTime) > Incremento_DebounceDelay) {
    
    if (Incremento_State != LerIncremento) {
      Incremento_State = LerIncremento;
    }
    if (LerIncremento != Incremento_Last_State) {
      Incremento_LastDebounceTime = millis();
    }
    if (Incremento_State != Incremento_Last_State && Incremento_State == LOW){
      NumeroEscolhido = (NumeroEscolhido + 1) % 32;                              // O jogador pode escolher números de 0 a 31
      mostrar_binario();
    }
  Incremento_Last_State = Incremento_State;
  }

  // Botão de Tentativa
  int LerTentativa = digitalRead(BotaoTentativa);
  if ((millis() - Tentativa_LastDebounceTime) > Tentativa_DebounceDelay) {       // Verifica se passou tempo suficiente desde a última leitura do botão
    
    if (Tentativa_State != LerTentativa) {                                       // Verifica se o estado do botão mudou e atualiza o estado dele de tiver mudado   
      Tentativa_State = LerTentativa;
    }
    if (LerTentativa != Tentativa_Last_State) {                                  // Reset no tempo do Debounce se houver uma mudança no estado do botão
      Tentativa_LastDebounceTime = millis();
    }
    if (Tentativa_State != Tentativa_Last_State && Tentativa_State == LOW){      // Verifica se o botão foi pressionado
    Serial.write(NumeroEscolhido);                                               // Envia a tentativa para o Arduíno mestre
    }
  Tentativa_Last_State = Tentativa_State;                                        // Atualiza o último estado do botão para o próximo loop
  }
}

// Função para ligar os Leds
void ligar_leds(){
   for (int led = 8; led <= 12; led++) {
    digitalWrite(led, HIGH);
  }
}

// Função para desligar os Leds
void desligar_leds(){
   for (int led = 8; led <= 12; led++) {
    digitalWrite(led, LOW);
  }
}

// Função para mostrar o número em binário nos Leds
void mostrar_binario(){
   for (int led = 8; led <= 12; led++) {
    digitalWrite(led, bitRead(NumeroEscolhido, led - 8)); 
   } 
}