import java.io.*;
import java.util.ArrayList;
import java.util.Scanner;

public class CheckSum{
    private String path;

    public CheckSum()
    {
        Byte[] txt = ReadTXT();

        SetParaty(txt);
        txt = GetCheckSum(txt);

        WriteTXT(txt);
    }

    private Byte[] ReadTXT() {
        Scanner scan = new Scanner(System.in);
        ArrayList<Byte> txtList = new ArrayList<Byte>();

        try {
            File input_file;

            do {
                System.out.print("input.txt가 위치한 경로를 입력해주세요 : ");
                path = scan.nextLine();

                input_file = new File(path, "input.txt");
            }while(!input_file.exists());

            //데이터를 읽어 byte로 저장
            FileReader txt_reader = new FileReader(input_file);
            for(Byte data; (data = (byte)txt_reader.read()) != -1; txtList.add(data));

            txt_reader.close();
        }catch (IOException e) {
            System.out.println(e);
        }

        return txtList.toArray(new Byte[txtList.size()]);
    }

    private void WriteTXT(Byte[] txtArray) {
        try {
            File output_file = new File(path, "output_checksum.txt");
            System.out.print("output.txt는 input.txt와 동일 경로에 저장됩니다.");

            FileWriter txt_writer = new FileWriter(output_file);
            for(int i = 0; i < txtArray.length; i++) txt_writer.write(txtArray[i]);

            txt_writer.close();
        }catch (IOException e) {
            System.out.println(e);
        }
    }

    private void SetParaty(Byte[] txtArray) {
        boolean paratyChecker;
        Byte bitChecker;

        for(int i = 0; i < txtArray.length ; i++) {

            bitChecker = 1;
            paratyChecker = (txtArray[i] & bitChecker) == bitChecker;
            //8번째 비트는 쉬프트 될것이기 때문에 제외
            ShowBits(txtArray[i]);
            System.out.print(" : ");

            for (int j = 1; j < 7; j++) {
                //다음 비트 체크
                bitChecker = (byte) (bitChecker << 1);
                paratyChecker = paratyChecker ^ ((txtArray[i] & bitChecker) == bitChecker);
            }

            //왼쪽 쉬프트
            txtArray[i] = (byte) (txtArray[i] << 1);
            if (paratyChecker) txtArray[i] = (byte) (txtArray[i] + 1);

            ShowBits(txtArray[i]);
            System.out.println();
        }
    }

    private void ShowBits(Byte txt) {
        for(int i = 128; i > 0; i = i/2)
        {
            if((txt&i) == 0) System.out.print(0);
            else System.out.print(1);
        }
    }
    private Byte[] GetCheckSum(Byte[] txtArray) {
        int checkSumNum = txtArray.length;
        int checkSumBit = 0;
        Byte checkSum[] = new Byte[2];

        for(int i = 0; i < checkSumNum/2; i++) {
            checkSumBit += ((txtArray[2*i]&0x00FF) * 256);
            checkSumBit += txtArray[2*i + 1]&0x00FF;
        }

        checkSum[0] = (byte)((checkSumBit >>> 8)^0x00FF);
        checkSum[1] = (byte)((checkSumBit)^0x00FF);

        System.out.print("CheckSum : ");
        ShowBits(checkSum[0]);
        ShowBits(checkSum[1]);
        System.out.println();

        return checkSum;
    }

    public static void  main(String[] agrs) {
        new CheckSum();
    }
}
