package de.oliverprobst.temperaturelog;

import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

public class Persist {

	private static String fileName;

	private static Writer fw = null;
	
	static {
		SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd__HH-mm-ss");
		Calendar cal = Calendar.getInstance();
		Date time = cal.getTime();

		fileName = sdf.format(time) + ".csv";
		
		try {
			fw = new FileWriter(fileName);
		} catch (IOException e) {
			System.err.println("Failure writing file");
			e.printStackTrace();
		}
	}
	


	public static String getFileName() {
		return fileName;
	}


	public static void writeTemperature(float value) {
		SimpleDateFormat timeFormat = new SimpleDateFormat("HH:mm:ss.SSS");
		Calendar cal = Calendar.getInstance();
		Date time = cal.getTime();
		try {
			fw.write(timeFormat.format(time) + "," + value);
			fw.write("\r\n");	
			fw.flush();
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

	public static void close (){

		if (fw != null)
			try {
				fw.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
	}
}
