import Vue from 'vue'
import App from './App.vue'
import FusionCharts from 'fusioncharts';
import Charts from 'fusioncharts/fusioncharts.charts';
import Widgets from 'fusioncharts/fusioncharts.widgets';
import PowerCharts from 'fusioncharts/fusioncharts.powercharts';
import FusionTheme from 'fusioncharts/themes/fusioncharts.theme.fusion';
import VueFusionCharts from 'vue-fusioncharts';
import TimeSeries from "fusioncharts/fusioncharts.timeseries";
Charts(FusionCharts);
PowerCharts(FusionCharts);
Widgets(FusionCharts);
FusionTheme(FusionCharts);
import VueApexCharts from 'vue-apexcharts'
Vue.use(VueApexCharts)

Vue.component('apexchart', VueApexCharts)
Vue.use(VueFusionCharts, FusionCharts,TimeSeries);

Charts(FusionCharts);

new Vue({
  el: '#app',
  render: h => h(App)
})
