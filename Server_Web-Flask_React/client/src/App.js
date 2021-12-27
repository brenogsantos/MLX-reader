import React from 'react'
import { render } from 'react-dom'
import "./App.css";
import { HeatMapGrid } from 'react-grid-heatmap'

const test_temps = ['5', '10', '15', '20', '25', '30', '35', '40','45', '50', '55', '60', '65', '70', '75', '80', '85', '90', '95', '100', '105', '110', '115', '120','125', '130', '135', '140', '145', '150', '155', '160']
const xLabels = new Array(16).fill(0).map((_, i) => `${i + 1}`)
const yLabels = ['1', '2', '3', '4']
var temps_mqtt = new Array(64)



const styles = (_x, _y, ratio) => {
  //console.log("_y: ", _y,  "_x: ", _x)
  //console.log("data: ", data_table[_x][_y])

 // console.log("em styles: ", temps_mqtt[_x][_y])
  let red = 0, green = 0, blue = 0;
  if(temps_mqtt[_x][_y] >= 50)
  {
    red = 255 
    green = 255 - (temps_mqtt[_x][_y])
    blue = 0
  } 
  else if (temps_mqtt[_x][_y] > 30)
  {
    red = 2 + ((temps_mqtt[_x][_y]))
    green =  255
    blue = 0
  }
  else if (temps_mqtt[_x][_y] >= 20)
  {
    red = 0
    green = 255 
    blue = 255 - ((temps_mqtt[_x][_y])*20)
  }
  else if (temps_mqtt[_x][_y] < 20)
  {
    red = 0
    green = 10 + ((temps_mqtt[_x][_y]))
    blue = 255
  }

  return {
    background: `rgb(${red}, ${green}, ${blue}, ${100})`,
    fontSize: '.8rem',
    color: `rgb(10, 10, 10, ${50 / 2 + 0.4})`
  };
};

class Timer extends React.Component {


  constructor(props) {

    super(props);
    this.state = {
      seconds: parseInt(props.startTimeInSeconds, 10) || 0,
      datas: parseInt()
    };

  }

  tick() {
    this.setState(state => ({
      seconds: state.seconds + 1
    }));

    fetch("/members").then(
      res => res.text()
    ).then(
      data => {
      //  console.log("data split: ", data.split("-"))
        const str_temp = data.split("-")

        temps_mqtt = new Array(yLabels.length)
        .fill(0)
        .map(() =>
          new Array(xLabels.length).fill(0).map(() => 
          0
          )
        )

        let aux = 0
        for(var i = 0; i < 4; i++) 
          for(var j = 0; j < 16; j++)
          {
           if((j == 0 || j == 1) && i == 0) temps_mqtt[i][j] = str_temp[aux++]
           else temps_mqtt[i][j] = str_temp[aux++]

          }
           
      }
    )

  }

  componentDidMount() {
    this.interval = setInterval(() => this.tick(), 100);
  }

  componentWillUnmount() {
    clearInterval(this.interval);
  }

  formatTime(secs) {

    let hours = Math.floor(secs / 3600);
    let minutes = Math.floor(secs / 60) % 60;
    let seconds = secs % 60;
    return [hours, minutes, seconds]
      .map(v => ('' + v).padStart(2, '0'))
      .filter((v, i) => v !== '00' || i > 0)
      .join(':');
  }


  render() {
    return (
      <div>
        Timer: {this.formatTime(this.state.seconds)}
        <div>

        </div>
        <div
          style={{
            width: '100%',
          }}
        >
          <HeatMapGrid
            data={temps_mqtt}
            xLabels={xLabels}
            yLabels={yLabels}
            // Reder cell with tooltip
            cellRender={(x, y, value) => (
              <div title={`Pos(${x}, ${y}) = ${value}`}>{value}</div>
            )}
            xLabelsStyle={(index) => ({
              fontSize: '.7rem',
              textTransform: 'uppercase',
              color: '#777'
            })}
            yLabelsStyle={() => ({
              fontSize: '.7rem',
              textTransform: 'uppercase',
              color: '#777'
            })}
            cellStyle={(_x, _y, ratio) => 
              styles(_x, _y, ratio)
            }
            cellHeight='4rem'
            xLabelsPos='bottom'
            onClick={(x, y) => alert(`Clicked (${x}, ${y})`)}
            yLabelsPos='right'
            square={0}
          />
        </div>

      </div>

    );
  }
}

export default Timer;
