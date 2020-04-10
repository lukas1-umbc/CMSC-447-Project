var dem_link = ''; //LINK TO DEMOCRAT GEOJSON DATA HERE
var repub_link = ''; //LINK TO REPUBLICAN GEOJSON DATA HERE

//setting source and format for democrat vectors
var dem_vectors = new ol.source.Vector({
    url: dem_link,
    format: new ol.format.GeoJSON()
});

//setting source and format for republican vectors
var repub_vectors = new ol.source.Vector({
    url: repub_link,
    format: new ol.format.GeoJSON()
});

//setting style for democrat vectors
var dem_style = new ol.style.Style({
    fill: new ol.style.Fill({
        color: 'rgba(73, 125, 193, 0.4)'
    }),
    stroke: new ol.style.Stroke({
        color: 'rgba(73, 125, 193, 1)',
        width: 5
    }),
    text: new ol.style.Text({
        font: '24px Calibri,sans-serif',
        fill: new ol.style.Fill({
            color: '#000'
        }),
        stroke: new ol.style.Stroke({
            color: '#fff',
            width: 3
        })
    })
});

//setting style for republican vectors
var repub_style = new ol.style.Style({
    fill: new ol.style.Fill({
        color: 'rgba(193, 73, 73, 0.4)'
    }),
    stroke: new ol.style.Stroke({
        color: 'rgba(193, 73, 73, 1)',
        width: 5
    }),
    text: new ol.style.Text({
        font: '24px Calibri,sans-serif',
        fill: new ol.style.Fill({
            color: '#000'
        }),
        stroke: new ol.style.Stroke({
            color: '#fff',
            width: 3
        })
    })
});

var map = new ol.Map({

    target: 'map',

    layers: [

      new ol.layer.Tile({
        source: new ol.source.OSM()
      }),

      //put democrat shapes on map
      new ol.layer.Vector({
        source: dem_vectors,
        style: dem_style
            //display name of party on each shape
            //function(feature) {
                //dem_style.getText().setText(feature.get('PARTY'))
                //return dem_style;
            //}
      }),

      //put republican shapes on map
      new ol.layer.Vector({
        source: repub_vectors,
        style: repub_style
            //function(feature) {
                //repub_style.getText().setText(feature.get('PARTY'))
                //return repub_style;
            //}
      })

    ],

    view: new ol.View({
        //lon, lat coordinates for maryland
        center: ol.proj.fromLonLat([-76.641273, 39.045753]),
        zoom: 8
   })

});