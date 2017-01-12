/** public sequence class that extends our abstract Element class,
    will use a linked list implementation */
public class Sequence extends Element {
	
	//do nothing, just return
	void element() {};	
	
	private Node<Element> head;
	private int seqCount;
	
	//sequence constructor, initalize to empty sequence
	public Sequence() {
		//head = new Node<Element>();
		seqCount = 0;
	}
	
	public Sequence(int integer) {
	    seqCount = integer;
	}
	
	//returns the first element in the sequence	
	Element first() {
		return head.data;
	}

	//returns the rest of the sequence aside from the first element
	Sequence rest() {
		Sequence rest = new Sequence(seqCount-1); //creates a new instance of sequence
		rest.head = this.head.getNext(); //set the head of rest equal to the next node in the sequence
		return rest;
	}

	//returns the length of the sequence
	int length() {
	    int ret = 0;
	    Node<Element> current = head;
			while (current != null)
		    {
			    ret++;
			    current = current.getNext();
		    }
		return ret;
	}
	
	//adds an element to our sequence
	void add(Element elm, int pos) {
        if(this.length() == 0){
            head = new Node<Element>(elm);
            seqCount++;
        }
        
        else if (pos == 0){
            Node<Element> temp = new Node<Element>(elm);
            temp.setNext(head);
            head = temp;
            seqCount++;
        }
        
		else if (pos <= seqCount && pos >= 0) {
			Node<Element> temp = new Node<Element>(elm);
			Node<Element> current = head;
			
			//iterate through list to pos
			for(int i = 0; i < pos-1 & current.getNext() != null; i++)
			{
				current = current.getNext();
			}
			
			temp.setNext(current.getNext()); //set the new node's next to the current node's next
			current.setNext(temp); //set the current node's next to the new node
		
			seqCount++;
		}
		else {
			System.out.println("The value of pos is not bounded by the length of our sequence.");
			System.exit(1);
		}
	}
	
	//removes an element at the specified position
	void delete(int pos) {

		Node<Element> onebefore = head;
		
		if( pos == 0){
		    head = onebefore.getNext();
		    seqCount--;
		}
		else{
		//iterate through list to one before pos
		for(int i = 0; i < pos - 1 & onebefore.getNext() != null; i++)
		{
			onebefore = onebefore.getNext();
		}
		
		//set the onebefore's next equal to the next of the deleted element
		onebefore.setNext(onebefore.getNext().getNext()); 
		seqCount--;
		}
		
	}

	//print a sequence by surrounding the value of the elements in [,]
	void Print() {
		
		Node<Element> current = head;

		System.out.print("[ ");
		while (current != null)
		{
			(current.getData()).Print();
			System.out.print(" ");
			current = current.getNext();
		}
		System.out.print("]");
	
	}
	
    //index allows access to an element at a certain position
    public Element index(int pos){
        Node<Element> current = head;
        
        for(int i = 0; i < pos & current.getNext() != null; i++)
			{
				current = current.getNext();
			}
		return current.data;
		//ADD AN ERROR MESSAGE IF NOT FOUND
    }
    
    public Sequence flatten(){
        //How do you identify a new Sequence?
        
        return new Sequence();
    }
    
    public Sequence copy(){
        return new Sequence();
    }

	/*-------------------------------
				Node Class
	-------------------------------*/
	class Node<Element> {
		
		//data members will be of type Element
		Node<Element> next;
		private Element data;
		
		//default constructor
		public Node() {}
		
		//element constuctor
		public Node(Element dataValue)
		{
			next = null;
			data = dataValue;
		}
		
		//constructor if we know where we want to point
		public Node(Element dataValue, Node<Element> nextValue) {
			next = nextValue;
			data = dataValue;
		}
		
		//get and set functions for data
		public Element getData() {
			return data;
		}
		public void setData(Element dataValue) {
			data = dataValue;
		}

		//get and set functions for the next node in the list
		public Node<Element> getNext() {
			return next;
		}
		public void setNext(Node<Element> nextValue) {
			next = nextValue;
		}

	}
}

