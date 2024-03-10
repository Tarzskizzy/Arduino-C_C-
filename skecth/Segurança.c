//projeto de robótica sistema de segurança de porta com sensor ultrasônico e teclado matricial
#include <liquidcrystal.h>
#include <keypad.h> // biblioteca do codigo
#include <string.h>
#include <stdbool.h>

const byte pino_trigger = a2;
const byte pino_echo = a1;
const byte linhas = 4; // linhas do teclado
const byte colunas = 4; // colunas do teclado
const byte buzzer = a5;
const byte pir = a2;
double dist;
int mov = 0;
int tempoecho;
const float velocidadesom = 0.034029;
int invasor = 0;

//                rs e d4 d5 d6 d7
liquidcrystal lcd(4,3,2,a4,a3,5);

int senha_estado = 0;

typedef struct {
  char valor[17];
  int pos;
  char censura[17];
  char resposta[17];
}password;

const char teclas_matriz[linhas][colunas] = { // matriz de caracteres (mapeamento do teclado)
  {'1', '2', '3', 'a'},
  {'4', '5', '6', 'b'},
  {'7', '8', '9', 'c'},
  {'*', '0', '#', 'd'}
};

const byte pinos_linhas[linhas] = {13, 12, 11, 10}; // pinos de conexao com as linhas do teclado
const byte pinos_colunas[colunas] = {9, 8, 7, 6}; // pinos de conexao com as colunas do teclado

keypad teclado_personalizado = keypad(makekeymap(teclas_matriz), pinos_linhas, pinos_colunas, linhas, colunas); // inicia teclado

password senha = {"",0};
//funções protótipo

void tocar();
void limpar();
void escrever(char tecla);
void verificar(bool estado);
void alterar_senha();

void setup() {
  pinmode(buzzer, output);
  pinmode(pir,input);
  pinmode(pino_trigger,output);
  pinmode(pino_echo,input);

  digitalwrite(pino_trigger, low);
  serial.begin(9600); // inicia porta serial
  lcd.begin(16,2);
  strcpy(senha.resposta,"1234");
}

void loop() {
  detectar_mov();
  verificar(senha_estado);
  lcd.setcursor(0,1);  
  char leitura_teclas = teclado_personalizado.getkey(); // atribui a variavel a leitura do teclado
  mov = digitalread(pir); //0 = travado
  if (leitura_teclas) { // se alguma tecla foi pressionada
    serial.println(senha.resposta);
    switch(leitura_teclas){
      case 'a':
      alterar_senha();
        break;
      case 'b':
      validar(senha.valor,senha.resposta);
        break;
      case 'c':
      limpar();
        break;
      case 'd':
        break;
      default:
        escrever(leitura_teclas);
      }
        tocar();
    }
    if (invasor ==1)
    {
      tone(buzzer,1200);
    }
    else{
      notone(buzzer);
    }
    
}

void tocar(){
  tone(buzzer,1000);
  delay(100);
  notone(buzzer);
}

void limpar(){
  lcd.setcursor(0,1);
  lcd.print("                ");
  memset(senha.valor,0,16);
  memset(senha.censura,0,16);
  senha.pos = 0;
}

void escrever(char tecla){
  if (senha.pos != 16){
      senha.valor[senha.pos] = tecla;
      senha.censura[senha.pos] = '*';
      senha.pos = senha.pos +1;
      lcd.print(senha.censura);
  }
}

void validar(char senha[17],char resposta[17]){
   int comparar = strcmp(senha,resposta);
   lcd.setcursor(0,0);
   switch(comparar){
    case 0:
      lcd.print("senha correta   ");
      delay(2000);
      senha_estado = trocar(senha_estado);
      limpar();
      break;
    default:
      lcd.print("senha incorreta ");
      delay(2000);
      limpar();
   }
}
void verificar(bool estado){
  lcd.setcursor(0,0);
  switch(estado){
    case true:
    lcd.print("modo destravado ");
    invasor = 0;
      break;
    default:
    lcd.print("modo travado    ");
      break;
  }
}

int trocar(int estado){
  if (estado == 1){
        estado = 0;
      }
      else{
        estado = 1;
      }
  return estado;
}

void alterar_senha(){
  int comparar = strcmp(senha.valor,senha.resposta);

  if (comparar == 0){
    lcd.setcursor(0,0);
    lcd.clear();
    lcd.print("digite n. senha ");
    tocar();
    limpar();
    bool looping = true;
    while (looping){
      lcd.setcursor(0,1);
      char leitura_teclas = teclado_personalizado.getkey(); // atribui a variavel a leitura do teclado
        if (leitura_teclas) { // se alguma tecla foi pressionada
    switch(leitura_teclas){
      case 'a':
      lcd.setcursor(0,0);
      lcd.print("tecla invalida  ");
      delay(500);
        break;
      case 'b':
      strcpy(senha.resposta, senha.valor);
      lcd.setcursor(0,0);
      lcd.print("senha alterada  ");
      delay(500);
      looping = false;
      limpar();
        break;
      case 'c':
      limpar();
        break;
      case 'd':
      lcd.setcursor(0,0);
      lcd.print("tecla invalida  ");
      delay(500);
        break;
      default:
        escrever(leitura_teclas);
      }
        tocar();
    }
    }
  }
  else{
    lcd.setcursor(0,0);
    lcd.print("senha incorreta");
    limpar();
    delay(500);
  }
  lcd.setcursor(0,0);

}

void detectar_mov(){
  digitalwrite(pino_trigger, high);
  delaymicroseconds(10);
  digitalwrite(pino_trigger, low);
  tempoecho = pulsein(pino_echo, high);
  dist = (tempoecho*velocidadesom)/2;
  serial.println(dist);

  if (dist < 20 && senha_estado == 0){
    invasor = 1;
  }
}
