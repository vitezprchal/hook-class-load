package com.example;

import java.lang.reflect.Constructor;

public class Main {
    public static void main(String[] args) throws Exception {
        System.out.println("loading in 10 seconds");
        Thread.sleep(10000);
        System.out.println("loading example");

        // dynamically load class using reflection
        SimpleClassLoader classLoader = new SimpleClassLoader(Main.class.getClassLoader());
        Class<?> exampleClass = classLoader.findClass("com.example.Example");
        Constructor<?> constructor = exampleClass.getDeclaredConstructor(String.class);
        Object example = constructor.newInstance("constructor with string arg");
        exampleClass.getMethod("call").invoke(example);
        System.out.println("done");
    }
}