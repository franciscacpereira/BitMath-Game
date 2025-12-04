/* Jogo BitMath - Trabalho de TI -
  Realizado por: Francicsa Camacho Pereira 
  13/11/2022 */

byte num1, num2; // números random 1 e 2
int op; // operação random que será escolhida (AND/OR/XOR)
int resultado; //resultado do jogo

/* variáveis para impedir que certas operações se executem múltiplas vezes */
int restart = 0; // variável que muda assim que começa o reset para que este se execute apenas uma vez antes do novo jogo
int acertaTempo = 0; // variável usada para executar cada temporzador apenas uma vez
// (se não no mesmo segundo era executado o 'if' várias vezes e fazia print várias vezes)

/* variáveis relativas ao botão */
const int buttonPin = 2;
int buttonState = 0;
int lastButtonState = LOW;
int contador = -1; // contador de cliques (tem valor negativo para não contar o primeiro clique)

/* variáveis de tempo */
int tempVarrimento = 300;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 30;
unsigned long tempoJogo = 7000;
unsigned long tempoInicio;
//**************************************************************************************
void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  // ligar os pinos dos leds todos
  for (int pin = 3; pin <= 13; pin++)pinMode(pin, OUTPUT);

  Serial.println("*** WELCOME TO THE BitMath GAME! ***");
  Serial.println("Press button to start!");
}
//**************************************************************************************
void loop() {

  int reading = !digitalRead(buttonPin); // leitura do valor dado pelo butão

  /* momento em que o botão é pressionado ou se altera o seu estado */
  if (reading != lastButtonState) {
    lastDebounceTime = millis(); // variável fica com o tempo em que o botão foi premido
  }
  /* 'if' que contabiliza quando o botão é premido (com debounce) */
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading; // atualizar o valor do buttonState
      if (buttonState == HIGH) {
        contador = contador + 1;
        if (contador == 0) {
          tempoInicio = millis(); // guarda o valor do início do jogo após o clique zero
          // se o reset foi usado anteriormente o valor do resart está a 1
          restart = 0; // repor o valor do restart a zero para que este possa ser executado de novo
          inicio_jogo();
          acertaTempo = 0; // repor o valor de referência dos temporizadores quando se começa o jogo de novo
        }
        // ignorar/não contabilizar o primeiro clique (só faz print dos cliques após o clique zero)
        // fazer print do nº do clique todas as outras vezes
        else {
          Serial.println((String) "Number: " + contador); 
        }
      }
    }
    // condição para se realizar o RESET 
    if (buttonState == HIGH && (millis() - lastDebounceTime > 1000) && restart == 0) {
      lastDebounceTime = millis(); // atualizar o lastDebounceTime
      modo_reset();
    }
  }
  /* condição para os temporizadores só se ativarem após premir o botão a primeira vez */
  if (contador != (-1)) {
    // o tempo atual (millis()) menos o tempo de inicio de jogo tem que ser igual a metade da duração do jogo
    if ((millis() - tempoInicio) == (tempoJogo * 0.50) && acertaTempo == 0) {
      Serial.println("Passou metade do tempo!");
      acertaTempo += 1; 
      // muda de valor para que mesmo que o programa corra rápido já não execute este 'if'
    }
    // o tempo atual (millis()) menos o tempo de inicio de jogo tem que ser igual a 75% da duração do jogo
    if ((millis() - tempoInicio) == (tempoJogo * 0.75) && acertaTempo == 1) {
      Serial.println("Passou 75% do tempo!");
      acertaTempo += 1;
      // muda de valor para que mesmo que o programa corra rápido já não execute este 'if'
    }
    // tempo de jogo acaba
    if (millis() - tempoInicio == tempoJogo) {
      if (contador == resultado) { // o número de cliques tem que ser igual ao resultado da operação
        Serial.println("WIN!");
        modo_vitoria();
      }
      else {
        Serial.println("LOSE :(");
        modo_derrota();
      }
    }
  }
  // update do último estado do botão
  lastButtonState = reading;
}

/***************************************************FUNÇÕES***************************************************/

//***Função de atribuição dos números random e da operação (AND/OR/XOR)***
void num_random() {
  randomSeed(analogRead(A0));
  num1 = random(0, 16);
  randomSeed(analogRead(A0));
  num2 = random(0, 16);
  randomSeed(analogRead(A0));
  op = random(0, 3);
}

//***Função que liga o led RGB consoante o valor que calhou em op (operação AND/OR/XOR)
void liga_rgb() {
  for (int i = 0; i < 3; i++) {
    // percorre os leds de 3 a 5
    digitalWrite(3 + i, op == i);
    // valores do i e a cor a que correspondem:
    // 0 = VERDE (XOR)
    // 1 = AZUL (OR)
    // 2 = VERMELHO (AND)
  }
}

//*** Função que desliga todas as cores do led RGB ***
void desliga_rgb() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(3 + i, LOW);
  }
}

//***Função para ligar os leds consoante os números e a operação escolhida pelo modo random ***
void liga_leds() {
  // acende os leds correspondentes do número 1
  for (int i = 0; i < 4; i++) {
    // usa um deslocamento à direita como máscara de corte para obter o valor de cada bit
    digitalWrite(6 + i, (num1 >> i) & 1);
  }
  // acende os leds correspondentes do número 2
  for (int i = 0; i < 4; i++) {
    // usa um deslocamento à direita como máscara de corte para obter o valor de cada bit
    digitalWrite(10 + i, (num2 >> i) & 1);
  }
  liga_rgb();
}

//***Função que desliga todos os leds (incluindo o RGB) ***
void desliga_leds() {
  for (int i = 3; i <= 13; i++) {
    digitalWrite(i, LOW);
  }
}

//***Função que faz o varrimento de fora para dentro ***
void varrimento_fora_dentro() {
  int sub = 7;
  for (int i = 13; i >= 10; i--) {
    digitalWrite(i, HIGH);
    digitalWrite(i - sub, HIGH);
    sub -= 2;
    // update do valor do sub para iterar entre os leds da seguinte forma:
    // 13 - 7 = 6 (13, 6)
    // 12 - 5 = 7 (12, 7)
    // ...
    delay(tempVarrimento);
  }
}

//***Função que faz o varrimento de dentro para fora ***
void varrimento_dentro_fora() {
  int sub = 1;
  for (int i = 10; i <= 13; i++) {
    digitalWrite(i, LOW);
    digitalWrite(i - sub, LOW);
    sub += 2;
    // update do valor do sub para iterar entre os leds da seguinte forma:
    // 10 - 1 = 9 (10, 9)
    // 11 - 3 = 8 (11, 8)
    // ...
    delay(tempVarrimento);
  }
}

//***Função que pisca o RGB enquanto faz o varrimento ***
void pisca_e_varre() {
  // função utilizada no reset
  desliga_leds();
  /* varrimento de fora para dentro */
  int sub = 7;
  for (int i = 13; i >= 10; i--) {
    digitalWrite(i, HIGH);
    digitalWrite(i - sub, HIGH);
    sub -= 2;
    delay(tempVarrimento);
    //parte relativa ao piscar do led RGB
    for (int i = 0; i < 3; i++) {
      digitalWrite(3 + i, !digitalRead(3 + i));
      // pisca branco (liga/desliga todos os pinos do RGB ao memso tempo)
    }
  }
  /* varrimento de dentro para fora */
  sub = 1;
  for (int i = 10; i <= 13; i++) {
    digitalWrite(i, LOW);
    digitalWrite(i - sub, LOW);
    sub += 2;
    delay(tempVarrimento);
    //parte relativa ao piscar do led RGB
    for (int i = 0; i < 3; i++) {
      digitalWrite(3 + i, !digitalRead(3 + i));
    }
  }
}

//***Função que executa o modo vitória (piscar 5 segundos e sequência de varrimento)***
void modo_vitoria() {
  // piscar as luzes que já estavam acessas (parte do blink durante 5 segundos)
  for (int k = 0; k < 5; k++) {
    liga_leds();
    delay(500);
    desliga_leds();
    delay(500);
  }
  for (int m = 0; m < 3; m++) { // executar a sequência do varrimento 3 vezes
    varrimento_fora_dentro();
    varrimento_dentro_fora();
  }
  // voltar ao valor inicial do contador para começar o jogo de novo
  contador = -1;
  Serial.println("\n*** WELCOME TO THE BitMath GAME! ***");
  Serial.println("Press button to start!");
}

//***Função que executa o modo derrota (apagar todos os leds e piscar o RGB)***
void modo_derrota() {
  desliga_leds();
  // pisca o led RGB com a cor que já tinha antes
  for (int r = 0; r < 5; r++) {
    liga_rgb();
    delay(500);
    desliga_rgb();
    delay(500);
  }
  // voltar ao valor inicial do contador para começar o jogo de novo
  contador = -1;
  Serial.println("\n*** WLCOME TO THE BitMath GAME! ***");
  Serial.println("Press button to start!");
}

//***Função que faz reset no jogo quando premido o butão por mais de 1 segundo ***
void modo_reset() {
  Serial.println("BitMath Resarting ...");
  // atualizar o valor do restart para que apenas execute o reset uma vez
  // mesmo que continue com o botão premido quando o novo jogo começa não faz reset
  restart = 1;
  pisca_e_varre();
  num_random(); // escolher novos números e operação aleatórios
  // voltar ao valor inicial do contador para começar o jogo de novo
  contador = -1;
  Serial.println("\n*** WLCOME TO THE BitMath GAME! ***");
  Serial.println("Press button to start!");
}

//***Função simplificada do começo do jogo ***
void inicio_jogo() {
  num_random();
  liga_leds();
  resposta();
}

//***Função para determinar o resultado da operação ***
void resposta() {
  // relacionar os valores de op (operação escolhida random) com os operadores AND/OR/XOR
  if (op == 0)resultado = num1 ^ num2; // VERDE = XOR
  if (op == 1) resultado = num1 | num2; // AZUL = OR
  if (op == 2) resultado = num1 & num2; // VERMELHO = AND
}