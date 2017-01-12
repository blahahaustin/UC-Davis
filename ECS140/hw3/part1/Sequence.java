/** public sequence class that extends our abstract Element class,
    will use a linked list implementation */
public abstract class Sequence extends Element {
	
	private Node<Element> head;
	private int listCount;
	
	//sequence constructor, initalize to empty sequence
	public Sequence() {
		head = new Node<Element>(null, null);
		listCount = 0;
	}

	/*-------------------------------
				Node Class
	-------------------------------*/
	class Node<Element> {
		
		//data members will be of type Element
		private Element data;
		private Node<Element> next;
		
		public Node(Element data, Node<Element> next)
		{
			this.data = data;
			this.next = next;
		}
	}
}
