#! /usr/bin/env ruby

class Greeter
	def initialize(name="world")
		@name = name.capitalize
	end
	def say_hi
		puts "Hello, #{@name}!"
	end
	def say_bye
		puts "\'Bye, #{@name}!"
	end
end

greeter = Greeter.new("bummy")
greeter.say_hi
greeter.say_bye

class Greeter
	attr_accessor :name
end

greeter.name = "bowdler"
greeter.say_hi
greeter.say_bye
