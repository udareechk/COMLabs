public class Node <T> {

	private T data;
	private Node <T> next;

	public Node(){

		this.data = null;
		this.next = null;
	}

	public Node(T data){

		this.data = data;
		
	}

	public Node(T data, Node <T> next){

		this.data = data;
		this.next = next;

	}

	public T getData(){

		return data;
	}

	public Node <T> getNextNode(){

		return next;
	}

	public void setData(T data){

		this.data = data;
	}

	public void setNextNode(Node <T> next){

		this.next = next;
	}

	public void printNode(){

		System.out.println(data);
	}



}