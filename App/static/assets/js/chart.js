console.log("Chart JS")
const dataTable = document.getElementById('data-table').getElementsByTagName('tbody')[0];
        
// Create line charts for Temperature, Moisture, and Humidity
const ctxTemperature = document.getElementById('temperature-chart').getContext('2d');
const ctxMoisture = document.getElementById('moisture-chart').getContext('2d');
const ctxHumidity = document.getElementById('humidity-chart').getContext('2d');
const ctxLightState = document.getElementById('lightstate-chart').getContext('2d');

ctxLightState.height = 200;


const lightStateChart = new Chart(ctxLightState, {
type: 'pie',
data: {
labels: ['true', 'false'], // Labels for different light states
datasets: [{
label: 'Rain Status',
data: [], // Count true and false occurrences in your data array
backgroundColor: [
    'rgba(0, 128, 0, 0.6)',  // Green for true
    'rgba(255, 0, 0, 0.6)',  // Red for false
],
borderWidth: 1
}]
}
});





// Create chart objects
const temperatureChart = new Chart(ctxTemperature, {
    type: 'line',
    data: {
        labels: [], // Add labels for x-axis if needed
        datasets: [{
            label: 'Temperature in Celsius',
            data: [],
            borderColor: 'rgba(255, 0, 0, 1)',
            borderWidth: 1,
            fill: false
        }]
    },
    options: {
        scales: {
            y: {
                beginAtZero: true
            }
        }
    }
});

const moistureChart = new Chart(ctxMoisture, {
    type: 'line',
    data: {
        labels: [],
        datasets: [{
            label: 'Air Quaility in PPM',
            data: [],
            borderColor: 'rgba(0, 0, 255, 1)',
            borderWidth: 1,
            fill: false
        }]
    },
    options: {
        scales: {
            y: {
                beginAtZero: true
            }
        }
    }
});

const humidityChart = new Chart(ctxHumidity, {
    type: 'line',
    data: {
        labels: [],
        datasets: [{
            label: 'Humidity',
            data: [],
            borderColor: 'rgba(0, 128, 0, 1)',
            borderWidth: 1,
            fill: false
        }]
    },
    options: {
        scales: {
            y: {
                beginAtZero: true
            }
        }
    }
});

let storedData = [];

function fetchDataAndUpdateTable() {
  $.get('/get_data', function (data) {
      if (data.length !== storedData.length) {
          storedData = data;
          let trueValues = 0;
          let falseValues = 0;
          let totalTemperature = 0;
          let totalMoisture = 0;
          let totalHumidity = 0;
          let totalCount = 0;

          // Clear existing chart data
          temperatureChart.data.labels = [];
          temperatureChart.data.datasets[0].data = [];
          moistureChart.data.labels = [];
          moistureChart.data.datasets[0].data = [];
          humidityChart.data.labels = [];
          humidityChart.data.datasets[0].data = [];
          lightStateChart.data.datasets[0].data = [];

          data.forEach((item) => {
              // Update the table
              const row = dataTable.insertRow();
              const temperatureCell = row.insertCell(0);
              const humidityCell = row.insertCell(1);
              const lightStateCell = row.insertCell(2);
              const moistureCell = row.insertCell(3);

              temperatureCell.textContent = item.temperature;
              humidityCell.textContent = item.humidity;
              lightStateCell.textContent = item.rain_status;
              moistureCell.textContent = item.air_quality;

              // Calculate averages
              totalTemperature += item.temperature;
              totalMoisture += item.humidity;
              totalHumidity += item.air_quality;
              totalCount++;

              item.rain_status ? trueValues++ : falseValues++;

              // Update the charts
              temperatureChart.data.labels.push(item.temperature);
              temperatureChart.data.datasets[0].data.push(item.temperature);
              moistureChart.data.labels.push(item.air_quality);
              moistureChart.data.datasets[0].data.push(item.air_quality);
              humidityChart.data.labels.push(item.humidity);
              humidityChart.data.datasets[0].data.push(item.humidity);
          });

          // Calculate averages
          const avgTemperature = totalTemperature / totalCount;
          const avgMoisture = totalMoisture / totalCount;
          const avgHumidity = totalHumidity / totalCount;

          // Update HTML elements with average values
          document.getElementById('avg-temperature').innerText = avgTemperature.toFixed(2); // Update with the correct element ID
          document.getElementById('avg-moisture').innerText = avgMoisture.toFixed(2); // Update with the correct element ID
          document.getElementById('avg-humidity').innerText = avgHumidity.toFixed(2); // Update with the correct element ID

          lightStateChart.data.datasets[0].data.push(trueValues);
          lightStateChart.data.datasets[0].data.push(falseValues);

          // Update the charts
          temperatureChart.update();
          moistureChart.update();
          humidityChart.update();
          lightStateChart.update();
      }
  });
}

// Fetch data initially and then refresh it every 5 seconds
fetchDataAndUpdateTable();
setInterval(fetchDataAndUpdateTable, 5000);


// Fetch data initially and then refresh it every 5 seconds
fetchDataAndUpdateTable();
setInterval(fetchDataAndUpdateTable, 5000);