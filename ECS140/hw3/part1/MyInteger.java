/** public integer class that extends our abstract Element class */
public abstract class MyInteger extends Element {

	//data member is a single integer
	private int myInt;
	
	//constructor initializes to 0
	MyInteger() {
		myInt = 0;
	}
	
	//function will return the integer value contained
	public int Get() {
		return myInt;
	}
	
	//function will set the integer value passed to it
	public void Set(int val) {
		myInt = val;
	}

}
