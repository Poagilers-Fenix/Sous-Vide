#include <Adafruit_SSD1306.h>
#define OLED_Address 0x3c
Adafruit_SSD1306 oled(128, 64);


//Declarando variáveis para facilitar o código por nome
// e não pelos números

int x=0, lastx=0, lasty=60, mais = 2, menos = 3, enter = 4, bomba = 9, 
aquecedor = 10, som = 11, led = 13, minDisplay = 0, minDisplayLast = 0,
TermistorPin = A0, tempSetada = 55, Vo;

float R1 = 10000, logR2, R2, T, Tc, Tf, temp, templast, timerlast,
c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;


// unsigned long é obrigatória para trabalhar com millis()
unsigned long minuto=0, tempoEnter=0, setTempo = 0;


//Configuração dos pinos
void setup(){	
	Serial.begin(9600);
	oled.begin(SSD1306_SWITCHCAPVCC, OLED_Address);
 	oled.clearDisplay();
  	oled.setTextSize(1);
  	oled.setTextColor(WHITE);

	pinMode(mais, INPUT_PULLUP);
	pinMode(menos, INPUT_PULLUP);
	pinMode(enter, INPUT_PULLUP);
	pinMode(bomba, OUTPUT);
	pinMode(aquecedor, OUTPUT);
	pinMode(som, OUTPUT);
	pinMode(led, OUTPUT);
	pinMode(9, OUTPUT);
	pinMode(13, OUTPUT);
	Serial.begin(9600);
}


void loop(){
	temp = readTemp(TermistorPin);
	oledDisplay(temp, minuto, minDisplay);
	
//capturar minuto. A variável se chama minuto mas o código é
//calculado em milissegundos
//Se eu apertar o botão "+" eu adiciono 1 minuto no timer
	if(digitalRead(mais) == LOW){
		minuto = minuto + 60000;
		minDisplay = minDisplay + 1;

//variável display para iminutar um timer. a cada 60 000 ms (1 minuto)
//adiciona 1 unidade nessa variável até 59

	if (minDisplay > 59){
		minDisplay = 0;
		}
	}

// Igual à variável acima só que com o botão "-"

	if(digitalRead(menos) == LOW && minuto > 0){
		minuto = minuto - 60000;
		minDisplay = minDisplay - 1;


	if (minDisplay < 0){
		minDisplay = 59;
		}
	}

	if(temp>0 && digitalRead(enter) == LOW && minuto >= 60000){
		digitalWrite(bomba, HIGH);
		digitalWrite(aquecedor, HIGH);
	}


	if (temp>=tempSetada){
		digitalWrite(aquecedor, LOW);
	}

	else if(temp<tempSetada && digitalRead(enter) == LOW){
		digitalWrite(bomba, HIGH);
		digitalWrite(aquecedor, HIGH);
	}
	
	else if(digitalRead(bomba)== HIGH && digitalRead(aquecedor) == LOW && temp<tempSetada){
		digitalWrite(aquecedor, HIGH);
	}

	if(digitalRead(enter) == LOW){
		tempoEnter = millis();
	}


	if(millis() - tempoEnter >= minuto && minuto >= 60000){
		digitalWrite(aquecedor, LOW);
		digitalWrite(bomba, LOW);


	if(digitalRead(led) == LOW && digitalRead(som) == LOW){
		digitalWrite(led, HIGH);
		digitalWrite(som, HIGH);
		}

	else{
		digitalWrite(led, LOW);
		digitalWrite(som, LOW);
		}
	}

	
	//imprime o tempo desde que o Arduino foi ativado na eletricidade
	Serial.print("Tempo atual: ");Serial.print((millis()-tempoEnter)/1000);Serial.println(" s");
	//Mostro o display de minutos para setar o timer
	Serial.print("Minuto: ");Serial.println(minDisplay);
	//Pego o valor em milissegundos, divido por 1000 e transformo em segundos.
	//Divido por 3600 e transformo em horas.
	Serial.print("Hora: ");Serial.println(minuto/1000/3600);
	//Imprimir temperatura
	printTemp(temp);
	//Imprimir temperatura setada
	Serial.print("Temperatura setada: ");Serial.print(tempSetada);Serial.println(" C");
	Serial.print("Status bomba: ");Serial.println(digitalRead(bomba));
	Serial.print("Status aquecedor: ");Serial.println(digitalRead(aquecedor));
	//Ciclo do arduino a cada 60 segundo para sincronizar com os timers;
	
	delay(1000);
	
	}

float readTemp(int ThermistorPin){	
	Vo = analogRead(ThermistorPin);
	R2 = R1 * (1023.0/(float)Vo - 1.0);
	logR2 = log(R2);
	T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
	Tc = T - 273.15;
	return Tc;

}

void printTemp(float Tc){
	
	Serial.print("Temperatura: ");
	Serial.print(Tc);
	Serial.println(" C");
	
}

void oledDisplay(float temp, unsigned long minuto, int minDisplay){
	oled.clearDisplay();
	
	oled.setCursor(0,0);
	oled.setTextColor(WHITE);
	oled.print("Temp: ");
	oled.println(temp);
	templast = temp;
	
	oled.println("");
	oled.print("Timer:  ");
	oled.print(minuto/1000/3600);oled.print(":");oled.print(minDisplay);
	timerlast = minuto/1000/3600;
	minDisplayLast = minDisplay;
	oled.display();
}
