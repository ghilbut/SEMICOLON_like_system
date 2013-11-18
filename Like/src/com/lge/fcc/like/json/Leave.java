package com.lge.fcc.like.json;

import java.io.Writer;

import com.thoughtworks.xstream.XStream;
import com.thoughtworks.xstream.io.HierarchicalStreamWriter;
import com.thoughtworks.xstream.io.json.JsonHierarchicalStreamDriver;
import com.thoughtworks.xstream.io.json.JsonWriter;

public class Leave {
	private Leave() {
		this.query = "leave";
	}
	
	public static String to() {
		XStream xstream = new XStream(new JsonHierarchicalStreamDriver() {
		    public HierarchicalStreamWriter createWriter(Writer writer) {
		        return new JsonWriter(writer, JsonWriter.DROP_ROOT_MODE);
		    }
		});
		return xstream.toXML(new Leave()).replace("\n", "").replace("\r", "");
	}
	
	private String query;
}
