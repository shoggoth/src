using System;

namespace HelloWorld {

	class HelloWorld {

		static void Main(string[] args) {

			Console.WriteLine("Hello, World");
			string inputString = Console.ReadLine();
			Console.WriteLine("Hello, " + inputString + " " + args[0]);
		}
	}
}
