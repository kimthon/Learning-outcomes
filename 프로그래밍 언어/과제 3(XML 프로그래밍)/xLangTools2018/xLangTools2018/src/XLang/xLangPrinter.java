package XLang;

import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.w3c.dom.Text;
import java.io.*;
import java.rmi.server.ExportException;

public class xLangPrinter {
	Element root;
	BufferedWriter bw;

	public xLangPrinter(Element e, String path) {
		root = e;
		try {
			bw = new BufferedWriter(new FileWriter(path));
		}
		catch (IOException error)
		{
			System.err.println(error);
			System.exit(1);
		}
	}

	public void printNode() {
	    Write("#include<stdio.h>");
	    WriteNewLine();

		printNode(root);
		try{
			bw.flush();
		}catch (Exception e) {
			System.err.println(e);
		}
	}
	private void printNode(Node e) {
		String name = e.getNodeName();
		if (name.equals("xLang18")) {
			//Write("i'm in <xLang18>...");
			Node child = e.getFirstChild();
			printNode(child);
			while ((child = child.getNextSibling()) != null)
				printNode(child);
		}
		else if (name.equals("Author")) {
				Write("//Author: " + e.getTextContent());
				WriteNewLine();
		}
		else if (name.equals("Function")||name.equals("Fun")) {
			if(name.equals("Function")) Write("int ");
			printNode(e.getFirstChild());
			printNode(e.getFirstChild().getNextSibling());
			if (e.getFirstChild().getNextSibling().getNextSibling() != null)
				printNode(e.getFirstChild().getNextSibling().getNextSibling());
		}
		else if (name.equals("Fname")) {
			Write(e.getTextContent());
			if(!e.getNextSibling().getNodeName().equals("Param")) Write("()");
		}
		else if (name.equals("Param")) {
			Node child = e.getFirstChild();
			Write("(");
			while (child != null) {
			    if(e.getParentNode().getNodeName().equals("Function"))	Write("int ");
				printNode(child);
				child = child.getNextSibling();
				if(child != null) Write(",");
			}
			Write(")");
		}
		else if (name.equals("Vname")) {
			Write(e.getTextContent());
		}
		else if (name.equals("Num")) {
			Write(e.getTextContent());
		}
		else if (name.equals("Statement")) {
			Node child = e.getFirstChild();
			Write("{");
			WriteNewLine();
			while (child != null) {
				printNode(child);
				child = child.getNextSibling();
			}
			Write("}");
			WriteNewLine();
		}
		else if (name.equals("Assign")) {
			if(e.getParentNode().getNodeName().equals("Statement")) Write("int ");
			Node child = e.getFirstChild();
			printNode(child);
			Write(" = ");
			printNode(child.getNextSibling());
			Write(";");
			WriteNewLine();
		}
		else if (name.equals("If")) {
			Write("if (");
			printNode(e.getFirstChild());
			Write(")");
			WriteNewLine();

			Node child = e.getFirstChild().getNextSibling();
			while(child != null)
			{
				if(child.getNodeName().equals("Else"))
				{
					Write("else ");
				}
				printNode(child);
				child = child.getNextSibling();
			}
		}
		else if (name.equals("EQ")) {
			printNode(e.getFirstChild());
			Write(" == ");
			printNode(e.getFirstChild().getNextSibling());
		}
		else if (name.equals("NE")) {
			printNode(e.getFirstChild());
			Write(" != ");
			printNode(e.getFirstChild().getNextSibling());
		}
		else if (name.equals("GE")) {
			printNode(e.getFirstChild());
			Write(" >= ");
			printNode(e.getFirstChild().getNextSibling());
		}
		else if (name.equals("GT")) {
			printNode(e.getFirstChild());
			Write(" > ");
			printNode(e.getFirstChild().getNextSibling());
		}
		else if (name.equals("LE"))	{
			printNode(e.getFirstChild());
			Write(" <= ");
			printNode(e.getFirstChild().getNextSibling());
		}
		else if(name.equals("LT"))
		{
			printNode(e.getFirstChild());
			Write(" < ");
			printNode(e.getFirstChild().getNextSibling());
		}
		else if(name.equals("While")) {
			Write("while");
			Write(" ( ");
			printNode(e.getFirstChild());
			Write(" ) ");
			WriteNewLine();
			printNode(e.getFirstChild().getNextSibling());
		}
		else if(name.equals("Do")||name.equals(("Then"))||name.equals("Else"))
		{
			Write("{");
			WriteNewLine();
			Node child = e.getFirstChild();
			while(child != null)
			{
				printNode(child);
				child = child.getNextSibling();
			}
			Write("}");
			WriteNewLine();
		}
		else if(name.equals("Input")) {
			Write("int ");
			printNode(e.getFirstChild());
			Write(";");
			WriteNewLine();
			Write("scanf(\"%d\", &");
			printNode(e.getFirstChild());
			Write(");");
			WriteNewLine();
		}
		else if(name.equals("Output")) {
			Write("printf(\"%d\\n\",");
			printNode(e.getFirstChild());
			Write(");");
			WriteNewLine();
		}
		else if(name.equals("Return")) {
			Write("return ");
			printNode(e.getFirstChild());
			Write(";");
			WriteNewLine();
		}
		else if(name.equals("Add")) {
			printNode(e.getFirstChild());
			Write(" + ");
			printNode(e.getFirstChild().getNextSibling());
		}
		else if(name.equals("Sub")) {
			printNode(e.getFirstChild());
			Write(" - ");
			printNode(e.getFirstChild().getNextSibling());
		}
		else if(name.equals("Mul")) {
			printNode(e.getFirstChild());
			Write(" * ");
			printNode(e.getFirstChild().getNextSibling());
		}
		else if(name.equals("Div")) {
			printNode(e.getFirstChild());
			Write(" / ");
			printNode(e.getFirstChild().getNextSibling());
		}
		else if(name.equals("Mod")) {
			printNode(e.getFirstChild());
			Write(" % ");
			printNode(e.getFirstChild().getNextSibling());
		}
	}

	private void Write(String string)
	{
		try {
			bw.write(string);
		}catch (Exception e)
		{
			System.err.println(e);
			System.exit(1);
		}
	}

	private void WriteNewLine()
	{
		try {
			bw.newLine();
		}catch(Exception e)
		{
			System.err.println(e);
			System.exit(1);
		}
	}

	public void printAll() {
		printAll(root, 0);
	}
	private void printAll(Node node, int indent) {
		if (node == null) return;
		NodeList nodes = node.getChildNodes();
		for (int c = 0; c < nodes.getLength(); c++) {
			for (int i=0; i<indent; i++)
				Write(" ");
			Write(nodes.item(c).getNodeName());
			if (nodes.item(c) instanceof Text) {
				//Text t = (Text)nodes.item(c);
				//Write(" \"" + t.getData() + "\"");
				Write(" \"" + nodes.item(c).getTextContent() + "\"");
			}
			System.out.println();
			printAll(nodes.item(c), indent+2);
		}
	}

}
