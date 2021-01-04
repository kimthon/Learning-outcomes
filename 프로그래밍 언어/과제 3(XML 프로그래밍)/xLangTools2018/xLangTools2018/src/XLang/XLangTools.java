package XLang;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.w3c.dom.Text;

import java.util.Scanner;
import java.io.File;

public class XLangTools {
	public static void main(String[] args) {
		Scanner scan;
		String path;

		do {
			System.out.print("파일이 위치한 경로를 입력해주세요 : ");
			scan = new Scanner(System.in);
			path = scan.nextLine();
		}while(IsFileExists(path));

		try {
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			DocumentBuilder builder = factory.newDocumentBuilder();
			Document doc = builder.parse(path);
			Element root = doc.getDocumentElement();

			removeEmpty(root);

			xLangPrinter printer = new xLangPrinter(root, path.substring(0, path.lastIndexOf('.')) + ".c");

			printer.printNode();

		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	static void removeEmpty(Node node) {
		if (node == null) return;
		NodeList nodes = node.getChildNodes();
		for (int c=nodes.getLength()-1; c>=0; c--) {
			if (nodes.item(c) instanceof Text) {
				String value = nodes.item(c).getNodeValue().trim();
				if (value.equals("")) {
					//System.out.println("Removing...");
					node.removeChild(nodes.item(c));
				}
			}
			else
				removeEmpty(nodes.item(c));
		}
	}

	static boolean IsFileExists(String path)
	{
		File file = new File(path);
		return !file.exists();
	}
}