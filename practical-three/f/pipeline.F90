module problem_mod
	use mpi
	implicit none

	! The maximum number of iterations
	integer, parameter :: MAX_ITERATIONS = 100000
	! Threshold of polution that needs to be cleaned
	real(kind=8), parameter :: THRESHOLD_VALUE = 18.75
	! How often to report the norm
	integer, parameter :: REPORT_NORM_PERIOD = 1000

	real(kind=8) :: active_time
 
contains

	subroutine run_pipeline()
		integer :: myrank, ierr, nx, ny, max_its, size
		real(kind=8) :: convergence_accuracy, summed_active_time, maximum_active_time, start_time
		character(len=32) :: arg
    
		call mpi_init(ierr)
		call mpi_comm_rank(MPI_COMM_WORLD, myrank, ierr)
		call mpi_comm_size(MPI_COMM_WORLD, size, ierr)
		start_time=MPI_Wtime()

		active_time=0.0

		if (command_argument_count() .lt. 4) then
			print *, &
				"You must provide four command line arguments, the global size in X,", &
				"the global size in Y, convergence accuracy and max number iterations"
			return
		end if

		call get_command_argument(1, arg)
		read(arg,*) nx
		call get_command_argument(2, arg)
		read(arg,*) ny
		call get_command_argument(3, arg)
		read(arg,*) convergence_accuracy
		call get_command_argument(4, arg)
		read(arg,*) max_its
    
		! Need to consider which MPI process will run which stage in pipeline and put it in here
		if (myrank == 0) then
			! First stage
		else if (myrank == 1) then
			! Second stage		
		end if
		! etc for other stages of the pipeline

		! Calculate and display the load imbalance factor here (recall it is the maximum utilisation / average utilisation) - I suggest a couple of reductions for this, 
		! the first one summing up all the active_time and the second one finding the max of the average time. 
		! Then on the root you can divide the summed average time by the
		! number of processes to find the overall global average
		if (myrank == 0) then			
			print *, "Overall runtime ", MPI_Wtime()-start_time, " seconds"
		end if

		call mpi_finalize(ierr)
	end subroutine run_pipeline 

	subroutine read_data_points()
		integer :: i, ierr
		character(len=65) :: file_name
		real(kind=8) :: values(60), startTime
    
		do i=5, command_argument_count()
			startTime=MPI_Wtime()
			call get_command_argument(i, file_name)
			open (unit=2, file=file_name, action='read', status='old', iostat=ierr)
			if (ierr == 0) then
				read (2,*) values
				close(2)
				active_time=active_time+(MPI_Wtime() - startTime)
				! Need to send the 60 values to the next stage in the pipeline
			else
				print *, "Warning: Can not find file ", file_name
			end if
		end do
		! Need to send a "poisoned pill" termination message to the next stage in the pipeline
	end subroutine read_data_points

	subroutine average_sample_values()
		integer :: ierr, num_elements, status(MPI_STATUS_SIZE), i
		real(kind=8) :: data_values(60), averaged_values(2), startTime

		do while (1==1)
			! Need to receive (blocking call) a message from the previous rank, also handle the poisoned pill termination case
			! The data from the previous stage is expected in the data_values array
			startTime=MPI_Wtime()
			! We suggest using MPI_Get_count on the status to figure out the number of elements (if it is zero then this is the poisoned pill)
			! then check this number of elements, if it is zero you can break out of the loop				

			averaged_values(1)=sum(data_values(:30))/30
			averaged_values(2)=sum(data_values(31:))/30
			active_time=active_time+(MPI_Wtime() - startTime)
			! Need to send the values to the next stage in the pipeline
		end do
		! Need to send a "poisoned pill" termination message to the next stage in the pipeline
	end subroutine average_sample_values  

	subroutine data_analysis(nx, ny)
		integer, intent(in) :: nx, ny
		real(kind=8), dimension(:,:), allocatable :: u_k
		real(kind=8) :: startTime
		integer :: ierr, num_elements, status(MPI_STATUS_SIZE), i, j, data_values(2)
		logical :: past_threshold

		allocate(u_k(0:ny+1, 0:nx+1))

		do while (1==1)
			! Need to receive (blocking call) a message from the previous rank, also handle the poisoned pill termination case
			! The data from the previous stage is expected in the u_k array			
			startTime=MPI_Wtime()
			! We suggest using MPI_Get_count on the status to figure out the number of elements (if it is zero then this is the poisoned pill)
			! then check this number of elements, if it is zero you can break out of the loop

			past_threshold=.false.
			data_values=0
			do i=0, nx+1
				do j=0, ny+1
					if (u_k(j, i) .ge. THRESHOLD_VALUE) then
						if (.not. past_threshold) then
							past_threshold=.true.
							data_values(1)=j
						end if
						data_values(2)=data_values(2)+1
					end if
				end do
			end do
			active_time=active_time+(MPI_Wtime() - startTime)
			! Need to send the values to the next stage in the pipeline
		end do
		! Need to send a "poisoned pill" termination message to the next stage in the pipeline
		deallocate(u_k)
	end subroutine data_analysis

	subroutine write_values()
		integer :: ierr, num_elements, status(MPI_STATUS_SIZE), i, data_values(2), file_number
		character(len=32) :: str_conv
		real(kind=8) :: startTime

		file_number=0
		do while (1==1)
			! Need to receive (blocking call) a message from the previous rank, also handle the poisoned pill termination case
			! The data from the previous stage is expected in the data_values array			
			startTime=MPI_Wtime()
			! We suggest using MPI_Get_count on the status to figure out the number of elements (if it is zero then this is the poisoned pill)
			! then check this number of elements, if it is zero you can break out of the loop

			if (file_number .lt. 10) then
				write(str_conv,'(I1)') file_number
			else
				write(str_conv,'(I2)') file_number
			end if
			open (unit=3, file="results/results_"//str_conv)
			write (3,*) "Sample number ", file_number
			write (3,*) "Pollution starts at grid point ", data_values(1)
			write (3,*) "Number of grid points over threshold that need cleaning ", data_values(2)
			close(3)
			print *, "Result file is: results_"//str_conv
			file_number=file_number+1
			active_time=active_time+(MPI_Wtime() - startTime)
		end do
	end subroutine write_values  

	subroutine run_solver(nx, ny, convergence_accuracy, max_its)
		integer, intent(in) :: nx, ny, max_its
		real(kind=8), intent(in) :: convergence_accuracy

		real(kind=8), dimension(:,:), allocatable :: u_k, u_kp1, temp
		real(kind=8) :: bnorm, rnorm, norm, boundary_values(2), startTime
		integer :: i, j, k, ierr, status(MPI_STATUS_SIZE), num_elements

		allocate(u_k(0:ny+1, 0:nx+1), u_kp1(0:ny+1, 0:nx+1), temp(0:ny+1, 0:nx+1))

		do while (1==1)
			! Need to receive (blocking call) a message from the previous rank, also handle the poisoned pill termination case
			! The data from the previous stage is expected in the boundary_values array, element 0 for the left item and 1 for the right item			
			startTime=MPI_Wtime()
			! We suggest using MPI_Get_count on the status to figure out the number of elements (if it is zero then this is the poisoned pill)
			! then check this number of elements, if it is zero you can break out of the loop
			bnorm=0.0
			rnorm=0.0

			call initialise_values(u_k, u_kp1, nx, ny, boundary_values(1), boundary_values(2))

			! Calculate the initial residual
			do i=1, nx
				do j=1, ny
					bnorm=bnorm+((u_k(j,i)*4-u_k(j-1,i)-u_k(j+1,i)-u_k(j,i-1)-u_k(j,i+1)) ** 2)
				end do
			end do
			bnorm=sqrt(bnorm)

			do k=0, MAX_ITERATIONS
				! Calculates the current residual norm
				do i=1, nx
					do j=1, ny
						rnorm=rnorm+((u_k(j,i)*4-u_k(j-1,i)-u_k(j+1,i)-u_k(j,i-1)-u_k(j,i+1)) ** 2)
					end do
				end do
				norm=sqrt(rnorm)/bnorm
				if (norm .lt. convergence_accuracy) exit
				if (max_its .gt. 0 .and. k .ge. max_its) exit

				! Do the Jacobi iteration
				do i=1, nx
					do j=1, ny
						u_kp1(j,i)=0.25 * (u_k(j-1,i) + u_k(j+1,i) + u_k(j,i-1) + u_k(j,i+1))
					end do
				end do

				temp=u_kp1
				u_kp1=u_k
				u_k=temp      				
				rnorm=0.0
			end do
			active_time=active_time+(MPI_Wtime() - startTime)
			! Need to send all the pollution values to the next stage in the pipeline
		end do
		! Need to send a "poisoned pill" termination message to the next stage in the pipeline
    		deallocate(u_k, u_kp1, temp)
	end subroutine run_solver

	! Initialises the arrays, such that u_k contains the boundary conditions at the start and end points and all other
	! points are zero. u_kp1 is set to equal u_k
	subroutine initialise_values(u_k, u_kp1, nx, ny, leftValue, rightValue)
		real(kind=8), intent(inout) :: u_k(0:ny+1, 0:nx+1), u_kp1(0:ny+1, 0:nx+1)
		real(kind=8), intent(in) :: leftValue, rightValue
		integer, intent(in) :: nx, ny

		! We are setting the boundary (left and right) values here, in the parallel version this should be exactly the same and no changed required

		u_k(0,:)=leftValue
		u_k(ny+1,:)=rightValue
		u_k(1:ny,:)=0.0_8
		u_kp1=u_k
	end subroutine initialise_values  
end module problem_mod

program diffusion
	use problem_mod
	implicit none

	call run_pipeline()
end program diffusion
