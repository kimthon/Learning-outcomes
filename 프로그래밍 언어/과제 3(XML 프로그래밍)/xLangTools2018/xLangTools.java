import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.w3c.dom.Text;

public class xLangTools {
	public static void main(String[] args) {
		try {
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			DocumentBuilder builder = factory.newDocumentBuilder();
			Document doc = builder.parse("../xlangprimenumber2018.xml");
			Element root = doc.getDocumentElement();

			removeEmpty(root);
			
			xLangPrinter printer = new xLangPrinter(root);
			printer.printAll();
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
}