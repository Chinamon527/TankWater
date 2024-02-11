import logo from "./logo.svg";
import "./App.css";
import { useState, useEffect } from "react";
import { ip } from "./config";

function App() {
  const [quality, setQuality] = useState(0);
  const [lv, setlv] = useState(0);
  const [servo, setservo] = useState(0);
  const [isFeedActive, setFeedActive] = useState(false);
  const [time, settime] = useState(new Date());

  const handleServoClick = () => {
    fetch(ip + "/do/" + 1)
      .then((respoone) => respoone.json())
      .then((data) => {});
  };

  useEffect(() => {
    const intervalId = setInterval(() => {
      fetch(ip + "/data")
        .then((respoone) => respoone.json())
        .then((data) => {
          setlv(data.level);
          setQuality(data.to);
          setFeedActive(data.servo == 1);
          setservo(data.servo);
          settime(new Date());
        });
    }, 1000); // ระยะเวลาที่คุณต้องการ (เช่น 1000 มิลลิวินาที = 1 วินาที)
    return () => {
      clearInterval(intervalId);
    };
  }, [setlv, setQuality]);

  return (
    <div className="main">
      <a className="fontsize">Tankwater</a>
      <h1>{time.toLocaleTimeString()}</h1>
      <div className="Row">
        <div className="box-n">
          <h1>Water Level</h1>
          <p style={{ fontSize: 50, fontweight: "bold" }}>{lv} %</p>
        </div>
        <div className="box-n">
          <h1>Water Quality</h1>
          <p style={{ fontSize: 50, fontweight: "bold" }}>{quality} %</p>
        </div>
        <div className="box-n">
          <h1>Feed</h1>
          <button
            className="button"
            disabled={isFeedActive}
            onClick={() => {
              if (servo == 1) {
                setservo(0);
              } else {
                setservo(1);
              }
              setFeedActive(!isFeedActive);
              handleServoClick();
            }}
          >
            {isFeedActive ? "OFF" : "ON"}
          </button>
          <p>{isFeedActive ? "status : ON" : "status : OFF"}</p>
          <div> </div>
        </div>
      </div>
    </div>
  );
}

export default App;
