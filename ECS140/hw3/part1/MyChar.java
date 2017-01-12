/** public char class that extends our abstract Element class */
public abstract class MyChar extends Element {

	//data member is a single char
	private char myChar;
	
	//constructor initializes to the char '0'
	MyChar() {
		myChar = '0';
	}

	//function will return the char value contained
	public char Get() {
		return myChar;
	}
	
	//function will set the char value passed to it
	public void Set(char val) {
		myChar = val;
	}

}
