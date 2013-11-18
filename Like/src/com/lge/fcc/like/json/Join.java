package com.lge.fcc.like.json;

import java.io.Writer;

import com.thoughtworks.xstream.XStream;
import com.thoughtworks.xstream.io.HierarchicalStreamWriter;
import com.thoughtworks.xstream.io.json.JsonHierarchicalStreamDriver;
import com.thoughtworks.xstream.io.json.JsonWriter;

public class Join {
	private Join(String user, String target) {
		this.query = "join";
		this.user = user;
		this.target = target;
	}
	public static String to(String user, String target) {
		XStream xstream = new XStream(new JsonHierarchicalStreamDriver() {
		    public HierarchicalStreamWriter createWriter(Writer writer) {
		        return new JsonWriter(writer, JsonWriter.DROP_ROOT_MODE);
		    }
		});
		return xstream.toXML(new Join(user, target)).replace("\n", "").replace("\r", "");
	}
	private String query;
	private String user;
	private String target;
}
