module mergesort_mod
	use qsort_c_module
	use ran2_mod
	implicit none  

contains

	recursive subroutine sort(data, dataLength, serial_threshold)
		integer, intent(in) :: dataLength, serial_threshold
		real(kind=8), dimension(:), intent(inout) :: data

		integer :: pivot

		if (dataLength < serial_threshold) then
			call QsortC(data)
		else
			pivot=dataLength/2

			call sort(data(:pivot), pivot, serial_threshold)
			call sort(data(pivot+1:), dataLength-pivot, serial_threshold) 

			call merge(data, pivot, dataLength)
		end if
	end subroutine sort

	subroutine merge(data, pivot, length)
		real(kind=8), dimension(:), intent(inout) :: data
		integer, intent(in) :: pivot, length

		real(kind=8), dimension(length) :: tempData
		integer i, pre_index, post_index

		pre_index=1
		post_index=pivot+1
		do i=1,length
			if (pre_index > pivot) then
				tempData(i)=data(post_index)
				post_index=post_index+1
			else if (post_index > length) then
				tempData(i)=data(pre_index)
				pre_index=pre_index+1
			else if (data(pre_index) .lt. data(post_index)) then
				tempData(i)=data(pre_index)
				pre_index=pre_index+1
			else
				tempData(i)=data(post_index)
				post_index=post_index+1
			end if
		end do
		data=tempData
	end subroutine merge
end module mergesort_mod

program mergesort_prog
	use mergesort_mod
	implicit none

	call entryPoint()
contains

	subroutine entryPoint()
		integer :: data_length, serial_threshold, int_display_data, start_time_values(8), end_time_values(8)
		integer(kind=8) :: start_ms, end_ms
		real(kind=8), dimension(:), allocatable :: data
		character(len=32) :: arg

		if (command_argument_count() .lt. 3) then
			print *, &
				"You must provide three command line arguments, the number of elements to be sorted, ", &
				"the serial threshold and whether to display raw and sorted data"
			return
		end if
		call get_command_argument(1, arg)
		read(arg,*) data_length
		call get_command_argument(2, arg)
		read(arg,*) serial_threshold
		call get_command_argument(3, arg)
		read(arg,*) int_display_data

		allocate(data(data_length))
		call generateUnsortedData(data, data_length)
		if (int_display_data == 1) then
			print *, "Unsorted data"
			call displayData(data, data_length)
		end if

		call date_and_time(values=start_time_values)
		call sort(data, data_length, serial_threshold)
		call date_and_time(values=end_time_values)

		if (int_display_data == 1) then
			print *, "Sorted data"
			call displayData(data, data_length)
		end if
		print *, "Runtime is ", getMSFromTime(end_time_values) - getMSFromTime(start_time_values), " ms"
		deallocate(data)
	end subroutine entryPoint  

	integer(kind=8) function getMSFromTime(time_values)
		integer :: time_values(8)

		getMSFromTime = ( time_values(5) )*60
	 	getMSFromTime = ( getMSFromTime + time_values(6) )*60
		getMSFromTime = ( getMSFromTime + time_values(7) )*1e3
		getMSFromTime = getMSFromTime + time_values(8) 
	end function getMSFromTime

	subroutine displayData(data, data_length)
		integer, intent(in) :: data_length
		real(kind=8), dimension(data_length), intent(in) :: data

		integer :: i
		do i=1, data_length
			write (6, "(f10.3)", advance='no') data(i)
		end do
		print *, ""
	end subroutine displayData

	subroutine generateUnsortedData(data, data_length)
		integer, intent(in) :: data_length
		real(kind=8), dimension(data_length), intent(out) :: data

		integer :: seed, i, clocktick

		call system_clock(clocktick)
		seed=-1-clocktick
		data(1)=ran2(seed)
		do i=2, data_length
			data(i)=ran2(seed)
		end do
	end subroutine generateUnsortedData
end program mergesort_prog
