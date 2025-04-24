package main

import (
	"fmt"
	"sort"
)

type Client struct {
	Ticket  string
	Duration int
}

type Queue struct {
	NumberOfWindows int
	Windows         [][]Client
	Visitors        []Client
}

func NewQueue(numWindows int) *Queue {
	return &Queue{
		NumberOfWindows: numWindows,
		Windows:         make([][]Client, numWindows),
	}
}

func (q *Queue) Enqueue(duration int, ticketNumber int) {
	ticket := fmt.Sprintf("T%d", ticketNumber)
	client := Client{Ticket: ticket, Duration: duration}
	q.Visitors = append(q.Visitors, client)
	fmt.Println(ticket)
}

func (q *Queue) Distribute() {
	load := make([]int, q.NumberOfWindows)

	// Сортировка посетителей по убыванию продолжительности для оптимизации распределения максимальной нагрузки
	sort.Slice(q.Visitors, func(i, j int) bool {
		return q.Visitors[i].Duration > q.Visitors[j].Duration
	})

	for _, v := range q.Visitors {
		minWindow := 0
		for i := 1; i < len(load); i++ {
			if load[i] < load[minWindow] {
				minWindow = i
			}
		}
		load[minWindow] += v.Duration
		q.Windows[minWindow] = append(q.Windows[minWindow], v)
	}

	for i := 0; i < q.NumberOfWindows; i++ {
		fmt.Printf("Окно %d (", i+1)
		time := 0
		notPrinted := true

		for _, v := range q.Windows[i] {
			if !notPrinted {
				fmt.Print(", ")
			}
			notPrinted = false
			fmt.Print(v.Ticket)
			time += v.Duration
		}

		fmt.Printf(") %d минут\n", time)
	}
}

func main() {
	var numberOfWindows int

	for {
		fmt.Print("Введите кол-во окон: ")
		if _, err := fmt.Scan(&numberOfWindows); err != nil || numberOfWindows <= 0 {
			fmt.Println("Ошибка! Пожалуйста, введите положительное число.")
			continue
		}
		break // Корректный ввод окон
	}

	queue := NewQueue(numberOfWindows)

	var command string
	ticketNumber := 1

	for {
		fmt.Scan(&command)

        switch command {
        case "ENQUEUE":
            var duration int

            // Убираем сообщение о вводе продолжительности обслуживания
            fmt.Scan(&duration)

            if duration <= 0 { // Проверка на некорректный ввод
                fmt.Println("Ошибка! Продолжительность должна быть положительным числом.")
                continue // Возврат к началу цикла для повторного ввода
            }

            queue.Enqueue(duration, ticketNumber)
            ticketNumber++

        case "DISTRIBUTE":
            if ticketNumber > 1 { // Проверка на наличие клиентов в очереди
                queue.Distribute()
                return

            } else {
                fmt.Println("Ошибка! Вы не добавили никого в очередь!")
                return
            }

        case "exit":
            return

        default:
            fmt.Println("Ошибка! Неизвестная команда.")
        }
    }
}
