package de.oliverprobst.temperaturelog;

import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadFactory;

import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.stage.Stage;

public class AnimatedLineChart extends Application {

	private static final int MAX_DATA_POINTS = 1280;
	private int xSeriesData = 0;
	private XYChart.Series<Number, Number> series1 = new XYChart.Series<>();
	private float lowest = 0;
	private float highest =100;
	private ExecutorService executor;
	private ConcurrentLinkedQueue<Number> dataQ1 = new ConcurrentLinkedQueue<>();

	private NumberAxis xAxis;
	private NumberAxis yAxis;
	

	private void init(Stage primaryStage) {

		xAxis = new NumberAxis(0, MAX_DATA_POINTS, MAX_DATA_POINTS / 10);
		xAxis.setForceZeroInRange(false);
		xAxis.setAutoRanging(false);
		xAxis.setTickLabelsVisible(false);
		xAxis.setTickMarkVisible(false);
		xAxis.setMinorTickVisible(false);
		
		yAxis = new NumberAxis ();
		yAxis.setForceZeroInRange(false);
		
		// Create a LineChart
		final LineChart<Number, Number> lineChart = new LineChart<Number, Number>(xAxis, yAxis) {
			// Override to remove symbols on each data point
			@Override
			protected void dataItemAdded(Series<Number, Number> series, int itemIndex, Data<Number, Number> item) {
			}
		};

		lineChart.setAnimated(false);
		lineChart.setTitle("Temperature");
		lineChart.setHorizontalGridLinesVisible(true);

		// Set Name for Series
		series1.setName("�C");

		// Add Chart Series
		lineChart.getData().addAll(series1);

		Scene scene = new Scene(lineChart);
		primaryStage.setScene(scene);
		
		String css = this.getClass().getResource("/styles/styles.css").toExternalForm(); 
	 	//scene.getStylesheets().clear();
		scene.getStylesheets().add(css);
	}

	@Override
	public void start(Stage stage) {
		stage.setTitle("Current temperature");
		init(stage);
		stage.show();

		executor = Executors.newCachedThreadPool(new ThreadFactory() {
			@Override
			public Thread newThread(Runnable r) {
				Thread thread = new Thread(r);
				thread.setDaemon(true);
				return thread;
			}
		});

		AddToQueue addToQueue = new AddToQueue();
		executor.execute(addToQueue);
		// -- Prepare Timeline
		prepareTimeline();
	}

	private TemperaturReceiver tr = new TemperaturReceiver();

	private class AddToQueue implements Runnable {
		public void run() {
			try {
				// add a item of random data to queue
				try {
					dataQ1.add(tr.readCurrentTemperature());
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}

				Thread.sleep(1000);
				executor.execute(this);
			} catch (InterruptedException ex) {
				ex.printStackTrace();
			}
		}
	}

	// -- Timeline gets called in the JavaFX Main thread
	private void prepareTimeline() {
		// Every frame to take any data from queue and add to chart
		new AnimationTimer() {
			@Override
			public void handle(long now) {
				addDataToSeries();
			}
		}.start();
	}

	private void addDataToSeries() {
		for (int i = 0; i < 20; i++) { // -- add 20 numbers to the plot+
			if (dataQ1.isEmpty())
				break;
			Number newVal =  dataQ1.remove();
			series1.getData().add(new XYChart.Data<>(xSeriesData++, newVal));
			if (newVal.floatValue() < lowest){
				lowest = newVal.floatValue();
			}
			if (newVal.floatValue() > highest){
				highest = newVal.floatValue();
			}
			
		}
		// remove points to keep us at no more than MAX_DATA_POINTS
		if (series1.getData().size() > MAX_DATA_POINTS) {
			series1.getData().remove(0, series1.getData().size() - MAX_DATA_POINTS);
		}		
		// update
		
		xAxis.setLowerBound(xSeriesData - MAX_DATA_POINTS);
		xAxis.setUpperBound(xSeriesData - 1);
		
		yAxis.setLowerBound(lowest -3);
		yAxis.setUpperBound(highest +3);
	}

	public static void main(String[] args) {
		
		launch(args);
	}
}