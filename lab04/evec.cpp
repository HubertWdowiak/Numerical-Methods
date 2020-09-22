// Kosek Krystian - 4

import java.util.Scanner;


class Stos{
    private int maksymalnyRozmiar;
    private int top;
    private char[] elementy;

    public Stos(int rozmiar){
        maksymalnyRozmiar = rozmiar;
        elementy = new char[maksymalnyRozmiar];
        top = -1; //nie zero bo wtedy oznaczaloby to ze jakis element juz tam jest
    }
    // #######dodaje na gore stosu
    public void Push(char x) {
        top = top + 1;
        elementy[top] = x;
    }
    //########usuwam ze stosu i zwracam
    public char Pop(){
        if(top >= 0){
            char x = elementy[top];
            top = top - 1;
            return x;
        }
        else {
            return '\0';
        }
    }
    //########zwraca element ktory jest na szczycie stosu
    public char Top(){
        if(top >= 0) {
            return elementy[top];
        }
        else {
            return '\0';
        }
    }

    public boolean Pusty()
    {
        if(top >= 0)
            return false;
        else
            return true;
    }
}

class StosStr{
    private int maksymalnyRozmiar;
    private int top;
    private String[] elementy;

    public StosStr(int rozmiar){
        maksymalnyRozmiar = rozmiar;
        elementy = new String[maksymalnyRozmiar];
        top = -1; //nie zero bo wtedy oznaczaloby to ze jakis element juz tam jest
    }
    // #######dodaje na gore stosu
    public void Push(String x) {
        top = top + 1;
        elementy[top] = x;
    }
    //########usuwam ze stosu i zwracam
    public String Pop(){
        if(top >= 0){
            String x = elementy[top];
            top = top - 1;
            return x;
        }
        else {
            return "";
        }
    }
    //########zwraca element ktory jest na szczycie stosu
    public String Top(){
        if(top >= 0) {
            return elementy[top];
        }
        else {
            return "";
        }
    }

    public boolean Pusty()
    {
        if(top >= 0)
            return false;
        else
            return true;
    }
}

class OnpToInf
{
    public int jakiPriorytet(char operator){
        if(operator =='=') {
            return 0;
        }
        else if(operator =='>' || operator =='<') {
            return 1;
        }
        else if(operator =='+' || operator=='-') {
            return 2;
        }
        else if(operator =='*' || operator =='/' || operator =='%') {
            return 3;
        }
        else if(operator =='^') {
            return 4;
        }
        else if(operator =='~') {
            return 5;
        }
        else if(operator >= 'a' && operator <= 'z') {
            return 6;
        }
        return -1;
    }

    public String ONPtoInfix(String wejscie) {
        String input = wejscie;
        String output = "INF: ";
        boolean error = false;
        int operatory = 0;
        int operandy = 0;

        Stos stosPriorytetow = new Stos(input.length());
        StosStr stosik = new StosStr(input.length());

        for( int i=0; i< input.length(); i++)
        {
            char ch = input.charAt(i);
            if( ch >= 'a' && ch <= 'z')
            {
                operandy ++;
                stosik.Push(Character.toString(ch));
                stosPriorytetow.Push((char)jakiPriorytet(ch));
            }
            else if (ch == '<' || ch == '>' || ch == '/' || ch == '%' || ch == '+' || ch == '=' || ch == '-' || ch == '*' || ch == '^')
            {
                String prawy = stosik.Pop();
                String lewy = stosik.Pop();

                int prawyPriorytet = stosPriorytetow.Pop();
                int lewyPriorytet = stosPriorytetow.Pop();

                if(jakiPriorytet(ch) >= prawyPriorytet)
                {
                    prawy = '(' + prawy + ')';
                }
                if(jakiPriorytet(ch) > lewyPriorytet)
                {
                    lewy = '(' + lewy + ')';
                }

                String wynik = lewy + ch + prawy;
                stosik.Push(wynik);
                stosPriorytetow.Push((char)jakiPriorytet(ch));
            }
            else if(ch == '~')
            {
                operatory++;
                String prawy = stosik.Pop();
                int prawyPriorytet = stosPriorytetow.Pop();

                if(jakiPriorytet(ch) >= prawyPriorytet)
                {
                    prawy = '(' + prawy + ')';
                }

                prawy = ch + prawy;
                stosik.Push( prawy);
                stosPriorytetow.Push((char)jakiPriorytet(ch));
            }
        }

        if(operandy != operatory+1)
            error = true;


        if(error)
            return "error";

        output += stosik.Pop();
        return output + '\n';
    }



}



class InfToOnp {

    public int jakiPriorytet(char operator){
        if(operator =='=') {
            return 0;
        }
        else if(operator =='>' || operator =='<') {
            return 1;
        }
        else if(operator =='+' || operator=='-') {
            return 2;
        }
        else if(operator =='*' || operator =='/' || operator =='%') {
            return 3;
        }
        else if(operator =='^') {
            return 4;
        }
        else if(operator =='~') {
            return 5;
        }
        else if(operator >= 'a' && operator <= 'z') {
            return 6;
        }
        return -1;
    }

    public boolean jakaLacznosc(char operator)
    {
        if(operator =='=') {
            return true;
        }
        else if(operator =='>' || operator =='<') {
            return false;
        }
        else if(operator =='+' || operator=='-') {
            return false;
        }
        else if(operator =='*' || operator =='/' || operator =='%') {
            return false;
        }
        else if(operator =='^') {
            return false;
        }
        else if(operator =='~') {
            return false;
        }
        return true;
    }

    public String InfixToONP(String wejscie) {
        String input = wejscie;
        String output = "ONP: ";
        int lewe_nawiasy = 0;
        int prawe_nawiasy= 0;
        int operatory = 0;
        int operandy = 0;
        Stos stosOperatorow = new Stos(input.length());
        boolean error = false;

        for (int i = 0; i <= input.length(); i++) {
            char ch=' ';
            if(i < input.length()) {ch = input.charAt(i);}
            char ch2;
            if (i<input.length() && i > 0 && ch >= 'a' && ch <= 'z') {
                ch2 = input.charAt(i - 1);
                if (ch2 >= 'a' && ch2 <= 'z') {
                    i = input.length();
                    error = true;
                }
            }
          /*  else if (i<input.length() && i > 0 && (ch == '<' || ch == '>' || ch == '/' || ch == '%' || ch == '+' || ch == '=' || ch == '-' || ch == '*' || ch == '^'))
            {
                ch2 = input.charAt(i - 1);
                if (ch2 == '<' || ch2 == '>' || ch2 == '/' || ch2 == '%' || ch2 == '+' || ch2 == '=' || ch2 == '-' || ch2 == '*' || ch2 == '^' || ch == '~')
                {
                    i = input.length();
                    error = true;
                }
            }
            
           */

            //1 przypadek
            if (i == input.length()) {

                while (!stosOperatorow.Pusty()) {
                    output += stosOperatorow.Pop();
                }
                break;
            }
            //2 przypadek
            else if (ch >= 'a' && ch <= 'z') {
                output += ch;
                operandy++;

            }
            //3 przypadek
            else if (ch == '(') {
                stosOperatorow.Push(ch);
                lewe_nawiasy++;
            }
            //4 przypadek
            else if (ch == ')') {
                prawe_nawiasy++;
                while (stosOperatorow.Top() != '(') {
                    output += stosOperatorow.Pop();
                }
                stosOperatorow.Pop();
            }
            //5 przypadek
            else if (ch == '<' || ch == '>' || ch == '/' || ch == '%' || ch == '+' || ch == '=' || ch == '-' || ch == '*' || ch == '^' || ch == '~') {
                operatory++;
                if(ch=='~')
                    operatory--;
                while (!stosOperatorow.Pusty()) {
                    if (((jakiPriorytet(stosOperatorow.Top()) >= jakiPriorytet(ch)) && (jakaLacznosc(ch) == false))
                            || ((jakiPriorytet(ch) < jakiPriorytet(stosOperatorow.Top())) && jakaLacznosc(ch) == true)) {
                        output += stosOperatorow.Pop();
                    } else {
                        break;
                    }
                }
                stosOperatorow.Push(ch);
            }
        }

        if(lewe_nawiasy != prawe_nawiasy || operandy!=operatory+1)
        {
            error = true;
        }

        if (error == true)
            return "error\n";
        else
            return output + '\n';
    }
};
public class Source {
    public static Scanner in = new Scanner(System.in);

    public static boolean automat(String x) {
        int dlugosc = x.length();
        boolean zero = true;
        boolean jeden = false;
        boolean dwa = false;
        char ch;
        for (int i = 0; i < dlugosc; i++) {
            ch = x.charAt(i);
            if (zero) {
                if (ch == '(') {
                }
                else if (ch >= 'a' && ch <= 'z') {
                    zero = false;
                    jeden = true;
                    dwa = false;
                }
                else if (ch == '~') {
                    zero = false;
                    jeden = false;
                    dwa = true;
                }
                else {
                    return false;
                }
            }
            else if (jeden) {
                if (ch == ')') {
                }
                else if (ch == '<' || ch == '>' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '^' || ch == '=') {
                    zero = true;
                    jeden = false;
                }
                else {
                    return false;
                }
            }
            else if (dwa) {
                if (ch == '~') {
                }
                else if (ch >= 'a' && ch <= 'z') {
                    jeden = true;
                    dwa = false;
                }
                else if (ch == '(') {
                    zero = true;
                    dwa = false;
                }
                else {
                    return false;
                }

            }
        }
        return true;

    }



    public static void main(String args[])
    {
        OnpToInf ONPTOINF = new OnpToInf();
        InfToOnp INFTOONP = new InfToOnp();

        int zestawy = in.nextInt();
        String wejscie = "";
        for( int i=0; i<zestawy; i++)
        {
            String wejscie2 = "";
            wejscie = in.nextLine();
            if(wejscie.startsWith("ONP: "))
            {
                for(int j=0; j<wejscie.length()-5; j++)
                {
                    wejscie2 += wejscie.charAt(j+5);
                }
                System.out.print(ONPTOINF.ONPtoInfix(wejscie2));
            }
            else if(wejscie.startsWith("INF: "))
            {
                for(int j=0; j<wejscie.length()-5; j++)
                {
                    wejscie2 += wejscie.charAt(j+5);
                }
                if(automat(wejscie2))
                    System.out.print(INFTOONP.InfixToONP(wejscie2));
                else
                    System.out.println("error");
            }
        }
    }

}
