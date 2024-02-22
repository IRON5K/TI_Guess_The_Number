const int BotaoInicio = 7;
int NumeroAleatorio;

int Inicio_State = LOW;
int Inicio_Last_State = LOW;
unsigned long Inicio_LastDebounceTime = 0;
unsigned long Inicio_DebounceDelay = 50;

void setup() {
  Serial.begin(9600);                                                
  for (int led = 8; led <= 12; led++) {
    pinMode(led, OUTPUT);                                            
  ligar_leds();                                  // Inicialmente os Leds estão Ligados
  }
  pinMode(BotaoInicio, INPUT_PULLUP);                                 
  randomSeed(analogRead(0));
}

void loop() {

  int LerInicio = digitalRead(BotaoInicio);

  if ((millis() - Inicio_LastDebounceTime) > Inicio_DebounceDelay) {        // Verifica se passou tempo suficiente desde a última leitura do botão
    
    if (Inicio_State != LerInicio) {                                        // Verifica se o estado do botão mudou e atualiza o estado dele de tiver mudado
      Inicio_State = LerInicio;
    }
    if (LerInicio != Inicio_Last_State) {                                   // Reset no tempo do Debounce se houver uma mudança no estado do botão
      Inicio_LastDebounceTime = millis();
    }
    if (Inicio_State != Inicio_Last_State && Inicio_State == LOW){          // Verifica se o botão foi pressionado
      NumeroAleatorio = random(0,32);                                       // Gera um número aleatório entre 0 e 31, inclusivamente (pois conseguimos representar 2^5 = 32 números diferentes)
      for (int led = 8; led <= 12; led++) {
        digitalWrite(led, bitRead(NumeroAleatorio, led-8));                 // Mostra o número aleatório nos LEDs em binário
      }
      Serial.write('S');                                                    // Envia 'S' para o Arduíno secundário indicando, que o jogo começou
    }
  Inicio_Last_State = Inicio_State;                                         // Atualiza o último estado do botão para o próximo loop
  }
  
  if (Serial.available() > 0) {                     // Verifica se há dados disponíveis para leitura na porta-série

    int resposta = Serial.read(); 

    if (NumeroAleatorio != resposta) {              // Se o número escolhido for errado, manda para o Arduíno Secundário 'N'
      Serial.write('N');                                              
    }
    else {                                          // Se o número escolhido não for errado (ou seja, é o número certo), manda para o Arduíno Secundário 'Y' e liga todos os Leds
      Serial.write('Y');                                              
      ligar_leds();
    }
  }
}

// Função para ligar os Leds
void ligar_leds(){
  for (int led = 8; led <= 12; led++) {
    digitalWrite(led, HIGH);
  }
}