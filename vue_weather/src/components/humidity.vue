<template>
  <div id="chart">
    <apexchart type="line" height="350" :options="chartOptions" :series="series"></apexchart>
  </div>
</template>

<script>
import axios from "axios";
import moment from "moment";

export default {
name: "humidity",
  mounted: function() {
    axios
      .get("https://api.openweathermap.org/data/2.5/onecall?lat=41.0351&lon=28.9833&units=metric&exclude=daily&appid=7f03f00418abad3a85f54dbec5e54c5f")
      .then(response => (this.info = response.data)).then(()=>{this.setChartData()})
    // this.setChartData();
  },
  methods: {
    setChartData: function() {
      var data = [];
      console.log(this.info)
      for (var i = 0; i < this.info.hourly.length; i++) {

        this.series[0].data.push(this.info.hourly[i].humidity)
        this.chartOptions.xaxis.categories.push(String(moment.unix(parseInt(this.info.hourly[i].dt)).format('MMM D, YYYY, HH:mm')))
      }


    },
  },
  data: function() {
    return {
      info : [],
      series: [{
        name: "Official Humidity %",
        data: []
      }
      ],
      chartOptions: {
        chart: {
          height: 350,
          type: 'line',
          zoom: {
            enabled: false
          },
        },
        dataLabels: {
          enabled: false
        },
        stroke: {
          width: [5, 7, 5],
          curve: 'straight',
          dashArray: [0, 8, 5]
        },
        title: {
          text: 'Humidity',
          align: 'left'
        },
        legend: {
          tooltipHoverFormatter: function(val, opts) {
            return val + ' - ' + opts.w.globals.series[opts.seriesIndex][opts.dataPointIndex] + ''
          }
        },
        markers: {
          size: 0,
          hover: {
            sizeOffset: 6
          }
        },
        xaxis: {
          type: 'datetime',
          categories: [
          ],
        },
        tooltip: {
          x: {
            format: "dd MMM yyyy"
          }},
        grid: {
          borderColor: '#f1f1f1',
        }
      },

    }
  },
}
</script>

<style scoped>

</style>
