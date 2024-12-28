package com.example;

import java.io.IOException;
import java.io.InputStream;

public class SimpleClassLoader extends ClassLoader {
    public SimpleClassLoader(ClassLoader parent) {
        super(parent);
    }

    @Override
    protected Class<?> findClass(String name) throws ClassNotFoundException {
        InputStream is = getClass().getResourceAsStream("/" + name.replace('.', '/') + ".class");

        if (is == null) {
            throw new ClassNotFoundException(name);
        }

        try {
            byte[] b = is.readAllBytes();
            return defineClass(name, b, 0, b.length);
        } catch (IOException e) {
            throw new ClassNotFoundException(name);
        }
    }

}
