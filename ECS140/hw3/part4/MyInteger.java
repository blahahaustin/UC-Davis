/** public integer class that extends our abstract Element class */
public class MyInteger extends Element {

	//do nothing, just return
	void element() {};		

	//data member is a single integer
	private int myInt;
	
	//constructor initializes to 0
	MyInteger() {
		myInt = 0;
	}
	
	//Get and Set methods
	public int Get() {
		return myInt;
	}
	public void Set(int val) {
		myInt = val;
	}
	
	//prints the corresponding integer value
	@Override
	public void Print() {
		System.out.print(myInt);
	}

}
