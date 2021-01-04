import javax.xml.parsers.documentbuilder;
import javax.xml.parsers.documentbuilderfactory;

import org.w3c.dom.document;
import org.w3c.dom.element;
import org.w3c.dom.node;
import org.w3c.dom.nodelist;
import org.w3c.dom.text;

public class xlangprinter {
	element root;
	public xlangprinter(element e) {
		root = e;
	}

	public void printnode() {
		printnode(root);
	}
	private void printnode(node e) {
		string name = e.getnodename();
		if (name.equals("xlang18")) {
			//system.out.println("i'm in <xlang18>...");
			node child = e.getfirstchild();
			printnode(child);
			while ((child = child.getnextsibling()) != null)
				printnode(child);
		}
		else if (name.equals("author")) {
			//system.out.println("i'm in <author>...");
			system.out.println("author: " + e.gettextcontent());
		}
		else if (name.equals("function")) {
			printnode(e.getfirstchild());
			printnode(e.getfirstchild().getnextsibling());
			if (e.getfirstchild().getnextsibling().getnextsibling() != null)
				printnode(e.getfirstchild().getnextsibling().getnextsibling());
		}
		else if (name.equals("fname")) {
			system.out.print(e.gettextcontent());
		}
		else if (name.equals("param")) {
			node child = e.getfirstchild();
			system.out.print("(");
			while (child != null) {
				printnode(child);
				system.out.print(",");
				child = child.getnextsibling();
			}
			system.out.println(")");
		}
		else if (name.equals("vname")) {
			system.out.print(e.gettextcontent());
		}
		else if (name.equals("num")) {
			system.out.print(e.gettextcontent());
		}
		else if (name.equals("statement")) {
			node child = e.getfirstchild();
			system.out.print("{");
			while (child != null) {
				printnode(child);
				system.out.println(";");
				child = child.getnextsibling();
			}
			system.out.println("}");
		}
		else if (name.equals("assign")) {
			node child = e.getfirstchild();
			printnode(child);
			system.out.print(" = ");
			printnode(child.getnextsibling());
		}
		else if (name.equals("if")) {
			system.out.print("if (");
			printnode(e.getfirstchild());
			system.out.print(") then {");
			printnode(e.getfirstchild().getnextsibling());
			system.out.println("}");
			if (e.getfirstchild().getnextsibling().getnextsibling() != null) {
				system.out.print("else {");
				printnode(e.getfirstchild().getnextsibling().getnextsibling());
				system.out.println("}");
			}
		}
		else if (name.equals("eq")) {
			printnode(e.getfirstchild());
			system.out.print(" == ");
			printnode(e.getfirstchild().getnextsibling());
		}

		//�̷������� ��ӵ˴θ�...

	}



	public void printall() {
		printall(root, 0);
	}
	private void printall(node node, int indent) {
		if (node == null) return;
		nodelist nodes = node.getchildnodes();
		for (int c = 0; c < nodes.getlength(); c++) {
			for (int i=0; i<indent; i++)
				system.out.print(" ");
			system.out.print(nodes.item(c).getnodename());
			if (nodes.item(c) instanceof text) {
				//text t = (text)nodes.item(c);
				//system.out.print(" \"" + t.getdata() + "\"");
				system.out.print(" \"" + nodes.item(c).gettextcontent() + "\"");
			}
			system.out.println();
			printall(nodes.item(c), indent+2);
		}
	}

}