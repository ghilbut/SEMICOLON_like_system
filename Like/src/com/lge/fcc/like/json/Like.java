package com.lge.fcc.like.json;

import java.io.Writer;

import com.thoughtworks.xstream.XStream;
import com.thoughtworks.xstream.io.HierarchicalStreamWriter;
import com.thoughtworks.xstream.io.json.JsonHierarchicalStreamDriver;
import com.thoughtworks.xstream.io.json.JsonWriter;

public class Like {
	private Like(String user, boolean like) {
		this.query = "like";
		this.user = user;
		this.like = like;
	}
	
	public static String to(String user, boolean like) {
		XStream xstream = new XStream(new JsonHierarchicalStreamDriver() {
		    public HierarchicalStreamWriter createWriter(Writer writer) {
		        return new JsonWriter(writer, JsonWriter.DROP_ROOT_MODE);
		    }
		});
		return xstream.toXML(new Like(user, like)).replace("\n", "").replace("\r", "");
	}
	
	private String query;
	private String user;
	private boolean like;
}
