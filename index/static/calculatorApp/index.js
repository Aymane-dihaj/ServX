const screenContent = document.getElementById("screen-content")
const screen = document.querySelector(".screen")
const buttonsEl = document.querySelectorAll(".buttons")
const equalEl = document.querySelector(".equal-btn")
const resetBtn = document.querySelector(".reset-btn")
const toggleButtons = document.getElementsByClassName("button")
const styleElement = document.getElementById("style-el")
let arr = [...toggleButtons]




const buttonsAnimation = [
    {transform: "translateY(5px)"},
];

const btnAnimationTiming = {
    duration: 100, 
}



let disabled = false
let count = 0

buttonsEl.forEach(btn =>
{
    btn.addEventListener("click", function()
    {
        btn.animate(buttonsAnimation, btnAnimationTiming)
            if (btn.value === "RESET")
            {
                screenContent.textContent = ""
                screenContent.style.color = ""
                screen.style.backgroundColor = ""
                screenContent.style.fontSize = ""
                disabled = false
                count = 0
            }
            else if (btn.value === '+' && disabled === false)
                screenContent.textContent += ' + '
            else if (btn.value === '-' && disabled === false)
                screenContent.textContent += ' - '
            else if (btn.value === 'x' && disabled === false)
                screenContent.textContent += ' x '
            else if (btn.value === '÷' && disabled === false)
                screenContent.textContent += ' ÷ '
            else if (btn.value === "DEL" && disabled === false)
            {
                let len = screenContent.textContent.length
                let res = screenContent.textContent.replace(screenContent.textContent.charAt(len - 1), "")
                screenContent.textContent = res
            }
            else if (btn.value === '=' && disabled === false)
            {
                let cleanedVersion = cleanInput(screenContent.textContent);
                calculate(cleanedVersion);
            }
            else
            {
                if (disabled === false)
                {
                    if (screenContent.textContent.length > 11 && count < 3)
                    {
                        screenContent.textContent += '.'
                        count++
                    }
                    else if (count < 3)
                        screenContent.textContent += btn.value
                }
            }
    })
})

function calculate(operation)
{
    try
    {
        screenContent.textContent = eval(operation)
        if (screenContent.textContent === "Infinity" || screenContent.textContent === "NaN")
            disabled = true
    }
    catch(error)
    {
        screenContent.textContent = "Math error  :("
        screenContent.style.fontSize = "25px"
        screenContent.style.color = "red"
        screen.style.backgroundColor = "rgba(50, 24, 24, 0.68)"
        disabled = true
    }
}



function cleanInput(input)
{
    let tmp = input.replace("x", "*");
    let result = tmp.replace("÷", "/")

    return result
}

arr.forEach((element, index) => {
    element.addEventListener("click", () =>
    {
        element.style.opacity = "1";
        if (index === 0)
            styleElement.setAttribute("href", "index.css");
        if (index === 1)
            styleElement.setAttribute("href", "light.css")
        arr.filter (function (item){
            return item != element
        })
        .forEach((item) => {
            item.style.opacity = "0";
        })
    })
})

