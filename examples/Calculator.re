open Revery;
open Revery.Core;
open Revery.UI;
open Revery.UI.Components;

module Row = (
  val component((render, ~children, ()) =>
    render(() => {
      let style = Style.make(
        ~alignItems=LayoutTypes.AlignStretch,
        ~flexDirection=LayoutTypes.Row,
        ~justifyContent=LayoutTypes.JustifyCenter,
        ()
      );
      <view style>
        ...children
      </view>;
    }, ~children)
  )
);

module Column = (
  val component((render, ~children, ()) =>
    render(() => {
      let style = Style.make(
        ~alignItems=LayoutTypes.AlignStretch,
        ~flexDirection=LayoutTypes.Column,
        ~justifyContent=LayoutTypes.JustifyCenter,
        ~backgroundColor=Colors.darkGrey,
        ()
      );
      <view style>
        ...children
      </view>;
    }, ~children)
  )
);

module Button = (
  val component((render, ~contents: string, ~onClick, ~children, ()) =>
    render(() => {
      let viewStyle = Style.make(
        ~backgroundColor=Colors.lightGrey,
        ~position=LayoutTypes.Relative,
        ~justifyContent=LayoutTypes.JustifyCenter,
        ~alignItems=LayoutTypes.AlignCenter,
        ~width=150,
        ~height=120,
        ~margin=10,
        (),
      );
      let textStyle = Style.make(
        ~color=Colors.black,
        ~fontFamily="Roboto-Regular.ttf",
        ~fontSize=32,
        ()
      );

      <Clickable onClick>
        <view style=viewStyle>
          <text style=textStyle> contents </text>
        </view>
      </Clickable>;
    }, ~children)
  )
);

module Display = (
  val component((render, ~display: string, ~curNum: string, ~children, ()) =>
    render(() => {
      let viewStyle = Style.make(
        ~backgroundColor=Colors.white,
        ~height=80,
        ~alignItems=LayoutTypes.AlignStretch,
        ~flexDirection=LayoutTypes.Column,
        ~justifyContent=LayoutTypes.JustifyFlexEnd,
        ()
      );
      let displayStyle = Style.make(
        ~color=Colors.black,
        ~fontFamily="Roboto-Regular.ttf",
        ~fontSize=20,
        ~margin=10,
        ()
      );
      let numStyle = Style.make(
        ~color=Colors.black,
        ~fontFamily="Roboto-Regular.ttf",
        ~fontSize=32,
        ~margin=10,
        ()
      );

      <view style=viewStyle>
        <text style=displayStyle> display </text>
        <text style=numStyle> curNum </text>
      </view>;
    }, ~children)
  )
);

type operator =
  [ `Nop | `Add | `Sub | `Mul | `Div ];

let showFloat(float) {
  let string = string_of_float(float);
  if (String.length(string) > 1
      && string.[String.length(string) - 1] == '.') {
    String.sub(string, 0, String.length(string) - 1);
  } else {
    string;
  };
}

module Calculator = (
  val component((render, ~children, ()) =>
    render(() => {
      /* The equation displayed */
      let (display, setDisplay) = useState("");
      /* The actual numerical result */
      let (result, setResult) = useState(0.);
      /* Operator */
      let (op, setOp) = useState(`Nop);
      /* Current number being typed */
      let (curNum, setCurNum) = useState("");

      let eval() {
        /* TODO: Check that there's already some input set */
        let (newDisplay, newResult) = switch (op) {
        | `Nop => (curNum, float_of_string(curNum))
        | `Add => (display ++ " + " ++ curNum, result +. float_of_string(curNum))
        | `Sub => (display ++ " - " ++ curNum, result -. float_of_string(curNum))
        | `Mul => (display ++ " × " ++ curNum, result *. float_of_string(curNum))
        | `Div =>
          if (float_of_string(curNum) != 0.) {
            (display ++ " ÷ " ++ curNum, result /. float_of_string(curNum));
          } else {
            /* TODO: Errors + clear button */
            ("Error: Divide by zero!", 0.);
          }
        };
        setResult(newResult);
        setDisplay(newDisplay);
        newResult;
      };

      let perform(newOp) {
        let _newResult = eval();
        setOp(newOp);
        setCurNum("");
      };

      let calculate() {
        let newResult = eval();
        setOp(`Nop);
        setCurNum(showFloat(newResult));
      };

      let clear() {
        setOp(`Nop);
        setCurNum("");
        setResult(0.);
        setDisplay("");
      };

      let backspace() {
        setCurNum(String.sub(curNum, 0, String.length(curNum) - 1));
      };

      <Column>
        <Display display curNum />
        <Row>
          <Button contents="C" onClick=(_ => clear()) />
          <Button contents="±" onClick=(_ => setCurNum("-" ++ curNum)) />
          <Button contents="<=" onClick=(_ => backspace()) />
          <Button contents="" onClick=(_ => ()) />
        </Row>
        <Row>
          <Button contents="7" onClick=(_ => setCurNum(curNum ++ "7")) />
          <Button contents="8" onClick=(_ => setCurNum(curNum ++ "8")) />
          <Button contents="9" onClick=(_ => setCurNum(curNum ++ "9")) />
          <Button contents="÷" onClick=(_ => perform(`Div)) />
        </Row>
        <Row>
          <Button contents="4" onClick=(_ => setCurNum(curNum ++ "4")) />
          <Button contents="5" onClick=(_ => setCurNum(curNum ++ "5")) />
          <Button contents="6" onClick=(_ => setCurNum(curNum ++ "6")) />
          <Button contents="×" onClick=(_ => perform(`Mul)) />
        </Row>
        <Row>
          <Button contents="1" onClick=(_ => setCurNum(curNum ++ "1")) />
          <Button contents="2" onClick=(_ => setCurNum(curNum ++ "2")) />
          <Button contents="3" onClick=(_ => setCurNum(curNum ++ "3")) />
          <Button contents="-" onClick=(_ => perform(`Sub)) />
        </Row>
        <Row>
          /* TODO: Only add one decimal */
          <Button contents="." onClick=(_ => setCurNum(curNum ++ ".")) />
          <Button contents="0" onClick=(_ => setCurNum(curNum ++ "0")) />
          <Button contents="=" onClick=(_ => calculate()) />
          <Button contents="+" onClick=(_ => perform(`Add)) />
        </Row>
      </Column>
    }, ~children)
  )
);

let init(app) {
  let window = App.createWindow(app, "Revery Calculator");

  let render() {
    <Calculator/>;
  };

  UI.start(window, render);
};

App.start(init);