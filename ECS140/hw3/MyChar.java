/** public char class that extends our abstract Element class */
public class MyChar extends Element {

	//do nothing, just return
	void element() {};

	//data member is a single char
	private char myChar;
	
	//constructor initializes to the char '0'
	MyChar() {
		myChar = '0';
	}

	//Get and Set methods
	public char Get() {
		return myChar;
	}
	public void Set(char val) {
		myChar = val;
	}

	//print the quoted value of MyChar object
	void print() {
	
	}

}
