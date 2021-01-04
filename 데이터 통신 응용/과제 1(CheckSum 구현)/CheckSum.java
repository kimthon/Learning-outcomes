import java.io.*;
import java.util.ArrayList;
import java.util.Scanner;

public class CheckSum{
    private String path;
    
    public void MakeCheckSum() {
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

		System.out.println("=====Even Parity 설정=====");
        for(int i = 0; i < txtArray.length ; i++) {
            bitChecker = 1;
            paratyChecker = false;

            ShowBits(txtArray[i]);
            System.out.print(" : ");

            for (int j = 0; j < 7; j++) {
                paratyChecker = paratyChecker ^ ((txtArray[i] & bitChecker) == bitChecker);
                bitChecker = (byte) (bitChecker << 1); // 다음 비트 체크
            }

            //왼쪽 쉬프트
            txtArray[i] = (byte) (txtArray[i] << 1);
            if (paratyChecker) txtArray[i] = (byte) (txtArray[i] + 1);

            int num = ShowBits(txtArray[i]);
			System.out.println("(" + num + ")");
        }
    }

    private int ShowBits(Byte txt) {
		int num = 0;
        for(int i = 128; i > 0; i = i/2)
        {
            if((txt&i) == 0) System.out.print(0);
            else {
				num += i;
				System.out.print(1);
			}
        }
		return num;
    }
    
    private Byte[] GetCheckSum(Byte[] txtArray) {
        int checkSumNum = txtArray.length;
        int checkSumBit = 0;
		int num = 0;
        Byte checkSum[] = new Byte[2];

		System.out.println("======16비트  나누기======");
        for(int i = 0; i < checkSumNum/2; i++) {
			num = ShowBits(txtArray[2*i])*256;
			num += ShowBits(txtArray[2*i + 1]);
			System.out.println("(" + num + ")");

            checkSumBit += num;

			//캐리 값 더해주기
			if(checkSumBit > 0xFFFF) checkSumBit = (checkSumBit & 0xFFFF) + 1;
        }

		if(checkSumNum%2 == 1) { 
			num = ShowBits(txtArray[checkSumNum - 1])*256;
			System.out.println("00000000(" + num + ")");
            checkSumBit += num;
		}

		//캐리 값 더해주기
		if(checkSumBit > 0xFFFF) checkSumBit = (checkSumBit & 0xFFFF) + 1;

		checkSumBit = (checkSumBit ^ 0xFFFF);

        checkSum[0] = (byte)((checkSumBit >>> 8)&0x00FF);
        checkSum[1] = (byte)((checkSumBit)&0x00FF);

		System.out.println("==========================");

        System.out.print("CheckSum : ");
        num = ShowBits(checkSum[0])*256;
        num += ShowBits(checkSum[1]);
		System.out.println("(" + num + ")");
		System.out.println("==========================");
        System.out.println();

        return checkSum;
    }

    public static void  main(String[] agrs) {
        CheckSum c = new CheckSum();
        c.MakeCheckSum();
    }
}
