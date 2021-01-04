import java.io.*;
import java.util.ArrayList;
import java.util.Scanner;

public class CRC{
    private String path;

    public CRC()
    {
        Byte[] txt = ReadTXT();

        SetParaty(txt);
        GetCRC(txt);

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

        return txtList.toArray(new Byte[txtList.size() + 2]);
    }

    private void WriteTXT(Byte[] txtArray) {
        try {
            File output_file = new File(path, "output_crc.txt");
            System.out.print("output_crc.txt는 input.txt와 동일 경로에 저장됩니다.");

            FileWriter txt_writer = new FileWriter(output_file);

            txt_writer.write(txtArray[txtArray.length-2]);
            txt_writer.write(txtArray[txtArray.length-1]);

            txt_writer.close();
        }catch (IOException e) {
            System.out.println(e);
        }
    }

    private void SetParaty(Byte[] txtArray) {
        boolean paratyChecker;
        Byte bitChecker;

        //마지막 2 바이트는 CRC가 들어가므로 Paritybit를 넣을 필요가 없음
        for(int i = 0; i < txtArray.length - 2; i++) {

            bitChecker = 1;
            paratyChecker = (txtArray[i] & bitChecker) == bitChecker;
            //8번째 비트는 쉬프트 될것이기 때문에 제외
            for (int j = 1; j < 7; j++) {
                //다음 비트 체크
                bitChecker = (byte) (bitChecker << 1);
                paratyChecker = paratyChecker ^ ((txtArray[i] & bitChecker) == bitChecker);
            }

            //왼쪽 쉬프트
            txtArray[i] = (byte) (txtArray[i] << 1);
            if (paratyChecker) txtArray[i] = (byte) (txtArray[i] + 1);
        }
    }

    private void ShowBits(Byte txt) {
        for(int i = 128; i > 0; i = i/2)
        {
            if((txt&i) == 0) System.out.print(0);
            else System.out.print(1);
        }
    }

    private void GetCRC(Byte[] txtArray) {
        Byte polynomial[] = new Byte[3];

        txtArray[txtArray.length - 2] = 0;
        txtArray[txtArray.length - 1] = 0;

        for(int i = 0; i < txtArray.length - 2 ; i++) {

            polynomial[0] = (byte)(0x00C0);
            polynomial[1] = (byte)(0x0002);
            polynomial[2] = (byte)(0x0080);


            for(int j = 1; j< 256; j *= 2) {
                if((txtArray[i]&(0x80/j)) == 0x80/j) {
                    txtArray[i] = (byte)(txtArray[i]^polynomial[0]);
                    txtArray[i + 1] = (byte)(txtArray[i + 1]^polynomial[1]);
                    txtArray[i + 2] = (byte)(txtArray[i + 2]^polynomial[2]);
                }

                PolyShifter(polynomial);
            }
        }

        System.out.printf("CRC : ");
        ShowBits(txtArray[txtArray.length - 2]);
        ShowBits(txtArray[txtArray.length - 1]);
        System.out.println();
    }


    private void PolyShifter(Byte[] polynomial) {
        for (int i = 2; i >= 0; i--) {
            polynomial[i] = (byte) ((polynomial[i]&0x00FF) >>> 1);
            if ((i > 0) && (polynomial[i - 1] % 2 == 1)) polynomial[i] = (byte) (polynomial[i] | 0x0080);
        }
    }

    public static void  main(String[] agrs) {
        new CRC();
    }
}