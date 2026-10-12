//Estados da maquina de estados
#define SEGUIR_LINHA 0
#define CURVA 1
#define PARADA 2

//Pinos
#define LED_DEBUG 13
#define IR_CONTADOR_LINHA 3
#define IR_OBSTACULO 2
#define IR_SEGUIDOR_DIREITO 11
#define IR_SEGUIDOR_ESQUERDO 12
#define PWM_MOTOR_DIREITO 5
#define PWM_MOTOR_ESQUERDO 6
#define MOTOR_DIREITO_IN_0 8
#define MOTOR_DIREITO_IN_1 7
#define MOTOR_ESQUERDO_IN_0 10
#define MOTOR_ESQUERDO_IN_1 9

//Variaveis globais
int estado = SEGUIR_LINHA;
int iCount = 0;
int isObstaculo = 0;


//===Variaveis locais do seguidor_de_linha()===

//Velocidades dos motores
#define VELOCIDADE_BASE 150
#define VELOCIDADE_CORRECAO 80

//=============================================


void setup()
{
  //Configuração dos pinos
  pinMode(LED_DEBUG, OUTPUT);
  pinMode(IR_CONTADOR_LINHA, INPUT);
  pinMode(IR_OBSTACULO, INPUT);
  pinMode(IR_SEGUIDOR_DIREITO, INPUT);
  pinMode(IR_SEGUIDOR_ESQUERDO, INPUT);
  pinMode(PWM_MOTOR_DIREITO, OUTPUT);
  pinMode(PWM_MOTOR_ESQUERDO, OUTPUT);
  pinMode(MOTOR_DIREITO_IN_0, OUTPUT);
  pinMode(MOTOR_DIREITO_IN_1, OUTPUT);
  pinMode(MOTOR_ESQUERDO_IN_0, OUTPUT);
  pinMode(MOTOR_ESQUERDO_IN_1, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(IR_OBSTACULO), obstaculo, CHANGE);
  attachInterrupt(digitalPinToInterrupt(IR_CONTADOR_LINHA), contador_linha, RISING);
}

void loop()
{
  switch(estado)
  {
    case SEGUIR_LINHA:
      seguidor_de_linha();
      break;
    case CURVA:
      realizar_a_curva();
      break;
    case PARADA:
      parar();
      break;
  }
}

void seguidor_de_linha()
{
  int sensorDir = digitalRead(IR_SEGUIDOR_DIREITO);
  int sensorEsq = digitalRead(IR_SEGUIDOR_ESQUERDO);
  
  // Linha centralizada
  if (sensorEsq == 1 && sensorDir == 1) {
    motorEsquerdo(VELOCIDADE_BASE);
    motorDireito(VELOCIDADE_BASE);
  }
  
  // Linha deslocada para a esquerda
  else if (sensorEsq == 0 && sensorDir == 1) {
    motorEsquerdo(VELOCIDADE_CORRECAO);
    motorDireito(VELOCIDADE_BASE);
  }

  // Linha deslocada para a direita
  else if (sensorEsq == 1 && sensorDir == 0) {
    motorEsquerdo(VELOCIDADE_BASE);
    motorDireito(VELOCIDADE_CORRECAO);
  }
}
  
  
void realizar_a_curva()
{
  //Fazer a curva
  estado = SEGUIR_LINHA;
}

  
  
void parar()
{
  analogWrite(PWM_MOTOR_DIREITO, 0);
  analogWrite(PWM_MOTOR_ESQUERDO, 0);
  
  digitalWrite(MOTOR_DIREITO_IN_0, LOW);
  digitalWrite(MOTOR_DIREITO_IN_1, LOW);
  digitalWrite(MOTOR_ESQUERDO_IN_0, LOW);
  digitalWrite(MOTOR_ESQUERDO_IN_1, LOW);
}

  
  
void contador_linha()
{
  iCount++;
  
  if(iCount == 3){
    estado = CURVA;
    iCount = 0;
  }
}

  
  
void obstaculo()
{
  isObstaculo =! isObstaculo;
  
  if(isObstaculo)
    estado = PARADA;
  else
    estado = SEGUIR_LINHA;
}

//===Funções dos motores===

void motorDireito(int velocidade) {
	if(velocidade > 0) { // "sinal" da velocidade define sentido de rotação
      digitalWrite(MOTOR_DIREITO_IN_0, HIGH);
      digitalWrite(MOTOR_DIREITO_IN_1, LOW);
    } else {
      digitalWrite(MOTOR_DIREITO_IN_0, LOW);
      digitalWrite(MOTOR_DIREITO_IN_1, HIGH);
    }
    
    analogWrite(PWM_MOTOR_DIREITO, abs(velocidade)); // 'abs' retorna o valor absoluto da velocidade informada
  }
  
void motorEsquerdo(int velocidade) {
    if(velocidade > 0) {
      digitalWrite(MOTOR_ESQUERDO_IN_0, HIGH);
      digitalWrite(MOTOR_ESQUERDO_IN_1, LOW);
    } else {
      digitalWrite(MOTOR_ESQUERDO_IN_0, LOW);
      digitalWrite(MOTOR_ESQUERDO_IN_1, HIGH);
    }
  
    analogWrite(PWM_MOTOR_ESQUERDO, abs(velocidade));
  }